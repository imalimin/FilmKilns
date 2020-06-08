/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "HwAndroidCodec.h"
#include "AlObjectGuard.h"
#include "AlLogcat.h"
#include <media/NdkImage.h>
#include "HwVideoFrame.h"
#include "libyuv.h"

#define TAG "HwAndroidCodec"

AlCodec *HwAndroidCodec::createDecoder(AlCodec::kID id) {
    HwAndroidCodec *c = new HwAndroidCodec(id);
    c->isEncodeMode = false;
    return c;
}

HwAndroidCodec::HwAndroidCodec(AlCodec::kID id)
        : AlCodec(id) {

}

HwAndroidCodec::~HwAndroidCodec() {
    release();
}

void HwAndroidCodec::release() {
    if (codec) {
        flush();
        AMediaCodec_stop(codec);
        AMediaCodec_delete(codec);
        codec = nullptr;
    }
    if (keyFrameBuf) {
        delete keyFrameBuf;
        keyFrameBuf = nullptr;
    }
    if (hwPacket) {
        delete hwPacket;
        hwPacket = nullptr;
    }
    if (outFrame) {
        delete outFrame;
        outFrame = nullptr;
    }
    delete mExtraData;
    mExtraData = nullptr;
}

HwResult HwAndroidCodec::configure(HwBundle &format) {
    AlCodec::configure(format);
    if (isEncodeMode && mReqExtraData) {
        mExtraData = HwAndroidCodec::makeExtraData(getCodecID(), format);
    }
    int32_t width = format.getInt32(KEY_WIDTH);
    int32_t height = format.getInt32(KEY_HEIGHT);
    int32_t bitRate = (int32_t) format.getInt32(KEY_BIT_RATE);
    this->keyFrameBuf = HwBuffer::alloc(static_cast<size_t>(width * height * 3 / 2));
    AMediaFormat *cf = AMediaFormat_new();
    if (AlCodec::kID::H264 == getCodecID()) {
        fps = format.getInt32(KEY_FPS);
        AMediaFormat_setString(cf, AMEDIAFORMAT_KEY_MIME, "video/avc");
        AMediaFormat_setInt32(cf, AMEDIAFORMAT_KEY_WIDTH, width);
        AMediaFormat_setInt32(cf, AMEDIAFORMAT_KEY_HEIGHT, height);
        if (isEncodeMode) {
            if (HwFrameFormat::HW_IMAGE_YV12 == (HwFrameFormat) format.getInt32(KEY_FORMAT)) {
                AMediaFormat_setInt32(cf, AMEDIAFORMAT_KEY_COLOR_FORMAT,
                                      COLOR_FormatYUV420Flexible);
            }
            AMediaFormat_setInt32(cf, AMEDIAFORMAT_KEY_BIT_RATE, bitRate);
            AMediaFormat_setInt32(cf, AMEDIAFORMAT_KEY_FRAME_RATE, fps);
            AMediaFormat_setInt32(cf, AMEDIAFORMAT_KEY_I_FRAME_INTERVAL, 3);// Seconds
        } else {
            HwBuffer *csd0Buf = dynamic_cast<HwBuffer *>(format.getObject("csd-0"));
            HwBuffer *csd1Buf = dynamic_cast<HwBuffer *>(format.getObject("csd-1"));
            AMediaFormat_setBuffer(cf, "csd-0", csd0Buf->data(), csd0Buf->size());
            AMediaFormat_setBuffer(cf, "csd-1", csd1Buf->data(), csd1Buf->size());
        }
    }
    const char *mime;
    AMediaFormat_getString(cf, AMEDIAFORMAT_KEY_MIME, &mime);
    if (isEncodeMode) {
        codec = AMediaCodec_createEncoderByType(mime);
    } else {
        codec = AMediaCodec_createDecoderByType(mime);
    }
    if (!codec) {
        AlLogE(TAG, "codec alloc failed");
        AMediaFormat_delete(cf);
        return false;
    }
    media_status_t ret = AMediaCodec_configure(codec, cf, nullptr, nullptr,
                                               isEncodeMode ? AMEDIACODEC_CONFIGURE_FLAG_ENCODE : 0);
    if (media_status_t::AMEDIA_OK != ret) {
        AlLogE(TAG, "codec alloc failed(%d)", ret);
        AMediaFormat_delete(cf);
        return false;
    }
    ret = AMediaCodec_start(codec);
    if (media_status_t::AMEDIA_OK != ret) {
        AlLogE(TAG, "codec alloc failed(%d)", ret);
        AMediaFormat_delete(cf);
        return false;
    }
    AMediaFormat_delete(cf);
    return Hw::SUCCESS;
}

HwResult HwAndroidCodec::start() {
    return Hw::SUCCESS;
}

HwResult HwAndroidCodec::process(HwAbsMediaFrame **frame, HwPacket **pkt) {
    uint8_t *data = nullptr;
    size_t size = 0;
    int64_t pts = 0;
    if (isEncodeMode) {
        if (frame && *frame) {
            data = (*frame)->data();
            size = (*frame)->size();
            pts = (*frame)->getPts();
        }
    } else {
        if (pkt && *pkt) {
            data = (*pkt)->data();
            size = (*pkt)->size();
            pts = (*pkt)->getPts();
        }
    }
    HwResult ret = push(data, size, pts);
    if (Hw::SUCCESS != ret) {
        AlLogE(TAG, "push failed");
    }
    HwResult ret1 = pop(2000);
    if (Hw::SUCCESS == ret1) {
        if (isEncodeMode) {
            *pkt = hwPacket;
        } else {
            *frame = outFrame;
        }
    } else if (Hw::IO_EOF == ret1) {
        return ret1;
    }
    return ret;
}

AlBuffer *HwAndroidCodec::getExtraData() {
    return mExtraData;
}

void HwAndroidCodec::flush() {
    media_status_t ret = AMediaCodec_flush(codec);
    if (media_status_t::AMEDIA_OK != ret) {
        AlLogE(TAG, "flush failed(%d)", ret);
    }
}

HwResult HwAndroidCodec::push(uint8_t *data, size_t size, int64_t pts) {
    ssize_t bufIdx = AMediaCodec_dequeueInputBuffer(codec, 2000);
    if (bufIdx < 0) {
        return Hw::FAILED;
    }
    if (nullptr == data) {
        AlLogI(TAG, "queue eof buffer.");
        media_status_t result = AMediaCodec_queueInputBuffer(codec, bufIdx, 0, 0, 0,
                                                             AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM);
        if (media_status_t::AMEDIA_OK != result) {
            AlLogE(TAG, "queue eof buffer failed(%d)", result);
        }
        return Hw::SUCCESS;
    }
    size_t bufSize = 0;
    auto buf = AMediaCodec_getInputBuffer(codec, bufIdx, &bufSize);
    if (bufSize > 0 && bufSize >= size) {
        bufSize = size;
        memcpy(buf, data, bufSize);
    }
    media_status_t result = AMediaCodec_queueInputBuffer(codec, bufIdx,
                                                         0, bufSize,
                                                         pts, 0);
    if (media_status_t::AMEDIA_OK != result) {
        AlLogE(TAG, "queue input buffer failed(%d)", result);
        return Hw::FAILED;
    }
    return Hw::SUCCESS;
}

HwResult HwAndroidCodec::pop(int32_t waitInUS) {
    AMediaCodecBufferInfo info;
    ssize_t bufIdx = AMediaCodec_dequeueOutputBuffer(codec, &info, waitInUS);
    switch (bufIdx) {
        case AMEDIACODEC_INFO_OUTPUT_BUFFERS_CHANGED: {
            AlLogI(TAG, "AMEDIACODEC_INFO_OUTPUT_BUFFERS_CHANGED");
            return Hw::FAILED;
        }
        case AMEDIACODEC_INFO_OUTPUT_FORMAT_CHANGED: {
            AlLogI(TAG, "AMEDIACODEC_INFO_OUTPUT_FORMAT_CHANGED");
            auto *format = AMediaCodec_getOutputFormat(codec);
            if (isEncodeMode) {
                uint8_t *csds[4] = {nullptr, nullptr, nullptr, nullptr};
                size_t csdLens[4] = {0, 0, 0, 0};
                AMediaFormat_getBuffer(format, "csd-0", reinterpret_cast<void **>(&csds[0]),
                                       &csdLens[0]);
                AMediaFormat_getBuffer(format, "csd-1", reinterpret_cast<void **>(&csds[1]),
                                       &csdLens[1]);
                AMediaFormat_getBuffer(format, "csd-2", reinterpret_cast<void **>(&csds[2]),
                                       &csdLens[2]);
                AMediaFormat_getBuffer(format, "csd-3", reinterpret_cast<void **>(&csds[3]),
                                       &csdLens[3]);


                size_t size = 0;
                if (csds[0]) {
                    size += csdLens[0];
                }
                if (csds[1]) {
                    size += csdLens[1];
                }
                if (csds[2]) {
                    size += csdLens[2];
                }
                if (csds[3]) {
                    size += csdLens[3];
                }
                if (size > 0) {
                    delete mExtraData;
                    mExtraData = AlBuffer::alloc(size);
                }

                if (csds[0]) {
                    mExtraData->put(csds[0], csdLens[0]);
                }
                if (csds[1]) {
                    mExtraData->put(csds[1], csdLens[1]);
                }
                if (csds[2]) {
                    mExtraData->put(csds[2], csdLens[2]);
                }
                if (csds[3]) {
                    mExtraData->put(csds[3], csdLens[3]);
                }
                AlLogI(TAG, "Got extra data size(%d), remaining(%d)", mExtraData->size(),
                       mExtraData->remaining());
                mExtraData->rewind();
            } else {
                int width = 0, height = 0;
                AMediaFormat_getInt32(format, AMEDIAFORMAT_KEY_WIDTH, &width);
                AMediaFormat_getInt32(format, AMEDIAFORMAT_KEY_HEIGHT, &height);
                AMediaFormat_getInt32(format, "stride", &stride);
                int32_t color = 0;
                AMediaFormat_getInt32(format, AMEDIAFORMAT_KEY_COLOR_FORMAT, &color);
                HwFrameFormat colorFmt;
                if (COLOR_FormatYUV420SemiPlanar == color) {
                    colorFmt = HwFrameFormat::HW_IMAGE_NV12;
                } else {
                    colorFmt = HwFrameFormat::HW_IMAGE_YV12;
                }
                outFrame = new HwVideoFrame(nullptr, colorFmt, width, height);
            }
            return Hw::FAILED;
        }
        case AMEDIACODEC_INFO_TRY_AGAIN_LATER: {
            AlLogI(TAG, "AMEDIACODEC_INFO_TRY_AGAIN_LATER");
            return Hw::FAILED;
        }
        case AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM: {
            AlLogI(TAG, "AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM");
            return Hw::FAILED;
        }
        default:
            break;
    }
    if (bufIdx < 0) {
        return Hw::FAILED;
    }
    size_t bufSize = 0;
    auto buf = AMediaCodec_getOutputBuffer(codec, bufIdx, &bufSize);
    if (!buf) {
        AMediaCodec_releaseOutputBuffer(codec, bufIdx, false);
        return Hw::FAILED;
    }
    if (info.flags & AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM) {
        AMediaCodec_releaseOutputBuffer(codec, bufIdx, false);
        return Hw::IO_EOF;
    }
    bool wrote = false;
    if (info.size > 0) {
        if (info.flags & BUFFER_FLAG_CODEC_CONFIG) {// config
            wrote = false;
        } else {
            if (isEncodeMode) {
                if (hwPacket) {
                    delete hwPacket;
                }
                memcpy(keyFrameBuf->data(), &info.size, 4);
                memcpy(keyFrameBuf->data() + 4, buf, info.size);
                hwPacket = HwPacket::wrap(keyFrameBuf->data(), info.size + 4,
                                          info.presentationTimeUs,
                                          info.presentationTimeUs,
                                          info.flags & BUFFER_FLAG_KEY_FRAME ? AV_PKT_FLAG_KEY : 0);
                hwPacket->setDuration(static_cast<int64_t>(AV_TIME_BASE /
                                                           (float) fps));
            } else {
                HwVideoFrame *videoFrame = dynamic_cast<HwVideoFrame *>(outFrame);
                videoFrame->setPts(info.presentationTimeUs);
                int32_t w = videoFrame->getWidth();
                int32_t h = videoFrame->getHeight();
                if (HwFrameFormat::HW_IMAGE_NV12 == videoFrame->getFormat()) {
                    int pixelCount = w * h;
                    libyuv::NV12ToI420(buf, stride,
                                       buf + stride * h, stride,
                                       videoFrame->data(),
                                       videoFrame->getWidth(),
                                       videoFrame->data() +
                                       pixelCount,
                                       videoFrame->getWidth() / 2,
                                       videoFrame->data() +
                                       pixelCount * 5 / 4,
                                       videoFrame->getWidth() / 2,
                                       videoFrame->getWidth(),
                                       videoFrame->getHeight());
                } else {
                    int32_t lines = h * 3 / 2;
                    for (int i = 0; i < lines; ++i) {
                        memcpy(videoFrame->data() + i * w,
                               buf + info.offset + i * stride * 3 / 2, w);
                    }
                }
            }
            wrote = true;
        }
    }
    //缓冲区使用完后必须把它还给MediaCodec，以便再次使用，至此一个流程结束，再次循环
    AMediaCodec_releaseOutputBuffer(codec, bufIdx, false);
    return wrote ? Hw::SUCCESS : Hw::FAILED;
}

AlBuffer *HwAndroidCodec::makeExtraData(AlCodec::kID id, HwBundle &format) {
    HwBundle bundle = format;
    AlBuffer *buf = nullptr;
    auto *codec = new HwAndroidCodec(id);
    codec->mReqExtraData = false;
    if (Hw::SUCCESS == codec->configure(bundle)) {
        uint32_t width = format.getInt32(KEY_WIDTH);
        uint32_t height = format.getInt32(KEY_HEIGHT);
        HwVideoFrame *frame = new HwVideoFrame(nullptr, HwFrameFormat::HW_IMAGE_YV12,
                                               width, height);
        int32_t offset = 0;
        memset(frame->data() + offset, 0, width * height);
        offset += width * height;
        memset(frame->data() + offset, 128, width * height / 4);
        offset += width * height / 4;
        memset(frame->data() + offset, 128, width * height / 4);
        frame->setPts(0);

        bool pushed = false;
        for (int i = 0; i < 60; ++i) {
            if (!pushed && Hw::SUCCESS == codec->push(frame->data(), frame->size(), 0)) {
                pushed = true;
                if (0 != i) {
                    AlLogW(TAG, "push index more than 1.");
                }
            }
            if (pushed) {
                codec->pop(5000);
                if (codec->getExtraData()) {
                    break;
                }
            }
        }
        delete frame;
        if (AlCodec::kID::H264 == id && nullptr == codec->getExtraData()) {
            AlLogE(TAG, "failed.");
            return nullptr;
        }

        buf = AlBuffer::alloc(codec->getExtraData()->size());
        buf->put(codec->getExtraData());
        buf->rewind();
    }
    codec->release();
    delete codec;
    return buf;
}