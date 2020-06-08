/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlAndroidCodecCompat.h"
#include "HwVideoFrame.h"
#include "AlObjectGuard.h"
#include "libyuv.h"

#define TAG "AlAndroidCodecCompat"

AlAndroidCodecCompat::AlAndroidCodecCompat(AlCodec::kID id)
        : AlCodec(id) {
    switch (id) {
        case AlCodec::kID::H264:
            bridge = new AlMediaCodecBridge("video/avc");
            break;
        default:
            break;
    }
}

AlAndroidCodecCompat::~AlAndroidCodecCompat() {
    release();
}

void AlAndroidCodecCompat::release() {
    if (bridge) {
        flush();
        bridge->stop();
        bridge->release();
    }
    delete bridge;
    bridge = nullptr;
    delete keyFrameBuf;
    keyFrameBuf = nullptr;
    delete hwPacket;
    hwPacket = nullptr;
    delete outFrame;
    outFrame = nullptr;
    delete mExtraData;
    mExtraData = nullptr;
}

HwResult AlAndroidCodecCompat::configure(AlBundle &format) {
    AlCodec::configure(format);
    if (isEncodeMode && mReqExtraData) {
        mExtraData = makeExtraData(getCodecID(), format);
    }
    int32_t width = format.get(KEY_WIDTH, INT32_MIN);
    int32_t height = format.get(KEY_HEIGHT, INT32_MIN);
    int32_t bitrate = format.get(KEY_BIT_RATE, INT32_MIN);
    this->keyFrameBuf = HwBuffer::alloc(static_cast<size_t>(width * height * 3 / 2));

    if (AlCodec::kID::H264 == getCodecID()) {
        fps = format.get(KEY_FPS, INT32_MIN);
        if (isEncodeMode) {
            if (Hw::SUCCESS != bridge->configure(width, height, bitrate, COLOR_FormatYUV420Flexible,
                                                 1, fps, CONFIGURE_FLAG_ENCODE)) {
                AlLogE(TAG, "codec configure failed.");
                release();
                return Hw::FAILED;
            }
        } else {
//            HwBuffer *csd0Buf = dynamic_cast<HwBuffer *>(format.getObject("csd-0"));
//            HwBuffer *csd1Buf = dynamic_cast<HwBuffer *>(format.getObject("csd-1"));
//            AMediaFormat_setBuffer(cf, "csd-0", csd0Buf->data(), csd0Buf->size());
//            AMediaFormat_setBuffer(cf, "csd-1", csd1Buf->data(), csd1Buf->size());
        }
    }
    if (Hw::SUCCESS != bridge->start()) {
        AlLogE(TAG, "codec start failed.");
        release();
        return Hw::FAILED;
    }
    return Hw::SUCCESS;
}

HwResult AlAndroidCodecCompat::start() {
    return Hw::SUCCESS;
}

HwResult AlAndroidCodecCompat::process(HwAbsMediaFrame **frame, HwPacket **pkt) {
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

void AlAndroidCodecCompat::flush() {
    if (Hw::SUCCESS != bridge->flush()) {
        AlLogE(TAG, "failed.");
    }
}

HwResult AlAndroidCodecCompat::push(uint8_t *data, size_t size, int64_t pts) {
    int bufIdx = bridge->dequeueInputBuffer(2000);
    if (bufIdx < 0) {
        return Hw::FAILED;
    }
    if (nullptr == data) {
        AlLogI(TAG, "queue eof buffer.");
        if (Hw::SUCCESS != bridge->queueInputBuffer(bufIdx, 0, 0, 0, BUFFER_FLAG_END_OF_STREAM)) {
            AlLogE(TAG, "queue eof buffer failed.");
        }
        return Hw::SUCCESS;
    }
    auto *buf = bridge->getInputBuffer(bufIdx);
    AlObjectGuard guard(reinterpret_cast<Object **>(&buf));
    if (buf->size() > 0 && buf->size() >= size) {
        buf->put(data, size);
    }
    if (Hw::SUCCESS != bridge->queueInputBuffer(bufIdx, 0, size, pts, 0)) {
        AlLogE(TAG, "queue input buffer failed.");
        return Hw::FAILED;
    }
    return Hw::SUCCESS;
}

HwResult AlAndroidCodecCompat::pop(int32_t waitInUS) {
    AlMediaCodecBridge::Info info;
    int bufIdx = bridge->dequeueOutputBuffer(info, waitInUS);
    switch (bufIdx) {
        case INFO_OUTPUT_BUFFERS_CHANGED: {
            AlLogI(TAG, "INFO_OUTPUT_BUFFERS_CHANGED");
            return Hw::FAILED;
        }
        case INFO_OUTPUT_FORMAT_CHANGED: {
            AlLogI(TAG, "INFO_OUTPUT_FORMAT_CHANGED");
            if (isEncodeMode) {
                auto *buf0 = bridge->getOutputFormatBuffer(KEY_CSD_0);
                auto *buf1 = bridge->getOutputFormatBuffer(KEY_CSD_1);
                auto *buf2 = bridge->getOutputFormatBuffer(KEY_CSD_2);
                auto *buf3 = bridge->getOutputFormatBuffer(KEY_CSD_3);
                AlObjectGuard guard0(reinterpret_cast<Object **>(&buf0));
                AlObjectGuard guard1(reinterpret_cast<Object **>(&buf1));
                AlObjectGuard guard2(reinterpret_cast<Object **>(&buf2));
                AlObjectGuard guard3(reinterpret_cast<Object **>(&buf3));

                size_t size = 0;
                if (buf0) {
                    size += buf0->size();
                }
                if (buf1) {
                    size += buf1->size();
                }
                if (buf2) {
                    size += buf2->size();
                }
                if (buf3) {
                    size += buf3->size();
                }
                if (size > 0) {
                    delete mExtraData;
                    mExtraData = AlBuffer::alloc(size);
                }

                if (buf0) {
                    mExtraData->put(buf0);
                }
                if (buf1) {
                    mExtraData->put(buf1);
                }
                if (buf2) {
                    mExtraData->put(buf2);
                }
                if (buf3) {
                    mExtraData->put(buf3);
                }
                AlLogI(TAG, "Got extra data size(%d), remaining(%d)", mExtraData->size(),
                       mExtraData->remaining());
                mExtraData->rewind();
            } else {
                int width = bridge->getOutputFormatInteger(KEY_WIDTH);
                int height = bridge->getOutputFormatInteger(KEY_HEIGHT);
                stride = bridge->getOutputFormatInteger("stride");
                int32_t color = bridge->getOutputFormatInteger("color-format");
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
        case INFO_TRY_AGAIN_LATER: {
            AlLogI(TAG, "INFO_TRY_AGAIN_LATER");
            return Hw::FAILED;
        }
        case BUFFER_FLAG_END_OF_STREAM: {
            AlLogI(TAG, "BUFFER_FLAG_END_OF_STREAM");
            return Hw::FAILED;
        }
        default:
            break;
    }
    if (bufIdx < 0) {
        return Hw::FAILED;
    }
    AlBuffer *buf = bridge->getOutputBuffer(bufIdx);
    AlObjectGuard guard(reinterpret_cast<Object **>(&buf));
    if (!buf) {
        bridge->releaseOutputBuffer(bufIdx, false);
        return Hw::FAILED;
    }
    if (info.flags & BUFFER_FLAG_END_OF_STREAM) {
        bridge->releaseOutputBuffer(bufIdx, false);
        return Hw::IO_EOF;
    }
    bool wrote = false;
    if (info.size > 0) {
        if (info.flags & BUFFER_FLAG_CODEC_CONFIG) {
            AlLogI(TAG, "Got extra data(%d)", info.size);
            wrote = false;
        } else {
            if (isEncodeMode) {
                if (hwPacket) {
                    delete hwPacket;
                }
                memcpy(keyFrameBuf->data(), &info.size, 4);
                memcpy(keyFrameBuf->data() + 4, buf->data(), info.size);
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
                    libyuv::NV12ToI420(buf->data(), stride,
                                       buf->data() + stride * h, stride,
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
    bridge->releaseOutputBuffer(bufIdx, false);
    return wrote ? Hw::SUCCESS : Hw::FAILED;
}

AlBuffer *AlAndroidCodecCompat::makeExtraData(AlCodec::kID id, AlBundle &format) {
    AlBundle bundle = format;
    AlBuffer *buf = nullptr;
    auto *codec = new AlAndroidCodecCompat(id);
    codec->mReqExtraData = false;
    if (Hw::SUCCESS == codec->configure(bundle)) {
        int32_t width = format.get(KEY_WIDTH, INT32_MIN);
        int32_t height = format.get(KEY_HEIGHT, INT32_MIN);
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
                if (codec->getFormat().contains(AlCodec::KEY_EXTRA_DATA)) {
                    break;
                }
            }
        }
        delete frame;
        if (AlCodec::kID::H264 == id && !codec->getFormat().contains(AlCodec::KEY_EXTRA_DATA)) {
            AlLogE(TAG, "failed.");
            return nullptr;
        }

        AlBuffer *extra = reinterpret_cast<AlBuffer *>(codec->getFormat()
                .get(AlCodec::KEY_EXTRA_DATA, AlLong::ZERO));
        buf = AlBuffer::alloc(extra->size());
        buf->put(extra);
        buf->rewind();
    }
    codec->release();
    delete codec;
    return buf;
}
