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

AlAndroidCodecCompat::AlAndroidCodecCompat(int32_t codecId, bool makeNalSelf)
        : HwAbsCodec(codecId),
          makeNalSelf(makeNalSelf) {
    bridge = new AlMediaCodecBridge("video/avc");
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
    for (int i = 0; i < 4; ++i) {
        if (buffers[i]) {
            delete buffers[i];
            buffers[i] = nullptr;
        }
    }
}

HwResult AlAndroidCodecCompat::configure(HwBundle &format) {
    HwAbsCodec::configure(format);
    if (encodeMode && !makeNalSelf) {
        auto *codec = new AlAndroidCodecCompat(codecId, true);
        if (Hw::SUCCESS == codec->configure(format)) {
            auto *buffer0 = codec->getExtraBuffer(HwAbsCodec::KEY_CSD_0);
            auto *buffer1 = codec->getExtraBuffer(HwAbsCodec::KEY_CSD_1);
            buffers[0] = HwBuffer::alloc(buffer0->size());
            buffers[1] = HwBuffer::alloc(buffer1->size());
            memcpy(buffers[0]->data(), buffer0->data(), buffer0->size());
            memcpy(buffers[1]->data(), buffer1->data(), buffer1->size());
        }
        codec->release();
        delete codec;
    }
    int32_t width = format.getInt32(KEY_WIDTH);
    int32_t height = format.getInt32(KEY_HEIGHT);
    int32_t bitrate = (int32_t) format.getInt32(KEY_BIT_RATE);
    this->keyFrameBuf = HwBuffer::alloc(static_cast<size_t>(width * height * 3 / 2));

    if (AlCodec::H264 == codecId) {
        fps = format.getInt32(KEY_FPS);
        if (encodeMode) {
            bridge->configure(width, height, bitrate, COLOR_FormatYUV420Flexible, 3, fps);
        } else {
//            HwBuffer *csd0Buf = dynamic_cast<HwBuffer *>(format.getObject("csd-0"));
//            HwBuffer *csd1Buf = dynamic_cast<HwBuffer *>(format.getObject("csd-1"));
//            AMediaFormat_setBuffer(cf, "csd-0", csd0Buf->data(), csd0Buf->size());
//            AMediaFormat_setBuffer(cf, "csd-1", csd1Buf->data(), csd1Buf->size());
        }
    }
    if (Hw::SUCCESS != bridge->start()) {
        LOGE(TAG, "codec start failed.");
        release();
        return false;
    }
    if (!encodeMode) {
        return Hw::SUCCESS;
    }
    if (makeNalSelf) {
        HwVideoFrame *frame = new HwVideoFrame(nullptr, HwFrameFormat::HW_IMAGE_YV12,
                                               width, height);
        int32_t offset = 0;
        memset(frame->data() + offset, 0, width * height);
        offset += width * height;
        memset(frame->data() + offset, 128, width * height / 4);
        offset += width * height / 4;
        memset(frame->data() + offset, 128, width * height / 4);
        frame->setPts(0);
        if (Hw::SUCCESS != push(frame->data(), frame->size(), 0)) {
            return Hw::FAILED;
        }
        for (int i = 0; i < 60; ++i) {
            pop(5000);
            if (buffers[0] && buffers[1]) {
                break;
            }
        }
        delete frame;
        if (AlCodec::H264 == codecId) {
            if (!buffers[0] || !buffers[1]) {
                return Hw::FAILED;
            }
        }

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
    if (encodeMode) {
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
        Logcat::i("hwvc", "AlAndroidCodecCompat::process push failed");
    }
    HwResult ret1 = pop(2000);
    if (Hw::SUCCESS == ret1) {
        if (encodeMode) {
            *pkt = hwPacket;
        } else {
            *frame = outFrame;
        }
    } else if (HwResult(-541478725) == ret1) {
        return ret1;
    }
    return ret;
}

int32_t AlAndroidCodecCompat::type() {
    switch (codecId) {
        case AlCodec::H264: {
            return 0;
        }
        default: {
            return 0;
        }
    }
}

HwBuffer *AlAndroidCodecCompat::getExtraBuffer(string key) {
    if (HwAbsCodec::KEY_CSD_0 == key) {
        return buffers[0];
    } else if (HwAbsCodec::KEY_CSD_1 == key) {
        return buffers[1];
    } else if (HwAbsCodec::KEY_CSD_2 == key) {
        return buffers[2];
    } else if (HwAbsCodec::KEY_CSD_3 == key) {
        return buffers[3];
    }
    return nullptr;
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
    if (!data) {
        if (Hw::SUCCESS != bridge->queueInputBuffer(bufIdx, 0, 0, 0, BUFFER_FLAG_END_OF_STREAM)) {
            LOGE(TAG, "queue eof buffer failed.");
        }
        return Hw::SUCCESS;
    }
    auto *buf = bridge->getInputBuffer(bufIdx);
    AlObjectGuard guard(reinterpret_cast<Object **>(&buf));
    if (buf->size() > 0 && buf->size() >= size) {
        buf->put(data, size);
    }
    if (Hw::SUCCESS != bridge->queueInputBuffer(bufIdx, 0, size, pts, 0)) {
        LOGE(TAG, "queue input buffer failed.");
        return Hw::FAILED;
    }
    return Hw::SUCCESS;
}

HwResult AlAndroidCodecCompat::pop(int32_t waitInUS) {
    AlMediaCodecBridge::Info info;
    int bufIdx = bridge->dequeueOutputBuffer(info, waitInUS);
    switch (bufIdx) {
        case INFO_OUTPUT_BUFFERS_CHANGED: {
            LOGI(TAG, "INFO_OUTPUT_BUFFERS_CHANGED");
            return Hw::FAILED;
        }
        case INFO_OUTPUT_FORMAT_CHANGED: {
            LOGI(TAG, "INFO_OUTPUT_FORMAT_CHANGED");
            if (encodeMode) {
                auto *buf0 = bridge->getOutputFormatBuffer(HwAbsCodec::KEY_CSD_0);
                auto *buf1 = bridge->getOutputFormatBuffer(HwAbsCodec::KEY_CSD_1);
                AlObjectGuard guard0(reinterpret_cast<Object **>(&buf0));
                AlObjectGuard guard1(reinterpret_cast<Object **>(&buf1));
                buffers[0] = HwBuffer::alloc(buf0->size());
                buffers[1] = HwBuffer::alloc(buf1->size());
                memcpy(buffers[0]->data(), buf0->data(), buf0->size());
                memcpy(buffers[1]->data(), buf1->data(), buf1->size());
            } else {
                int width = bridge->getOutputFormatInteger(HwAbsCodec::KEY_WIDTH);
                int height = bridge->getOutputFormatInteger(HwAbsCodec::KEY_HEIGHT);
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
            LOGI(TAG, "INFO_TRY_AGAIN_LATER");
            return Hw::FAILED;
        }
        case BUFFER_FLAG_END_OF_STREAM: {
            LOGI(TAG, "BUFFER_FLAG_END_OF_STREAM");
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
        return HwResult(-541478725);
    }
    bool wrote = false;
    if (info.size > 0) {
        if (info.flags & BUFFER_FLAG_CODEC_CONFIG) {// config
            wrote = false;
        } else {
            if (encodeMode) {
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
