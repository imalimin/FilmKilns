/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "HwAndroidCodec.h"
#include "Logcat.h"
#include <media/NdkImage.h>
#include "../../../include/HwVideoFrame.h"

const int32_t HwAndroidCodec::HW_ANDROID_AVC = AV_CODEC_ID_H264;

HwAndroidCodec::HwAndroidCodec(int32_t codecId) : HwAbsCodec(codecId) {

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
    if (configBuf) {
        delete configBuf;
        configBuf = nullptr;
    }
    if (keyFrameBuf) {
        delete keyFrameBuf;
        keyFrameBuf = nullptr;
    }
    for (int i = 0; i < 4; ++i) {
        if (buffers[i]) {
            delete buffers[i];
            buffers[i] = nullptr;
        }
    }
}

void HwAndroidCodec::flush() {

}

HwResult HwAndroidCodec::configure(HwBundle *format) {
    HwAbsCodec::configure(format);
    int32_t width = format->getInt32(KEY_WIDTH);
    int32_t height = format->getInt32(KEY_HEIGHT);
    this->keyFrameBuf = HwBuffer::alloc(static_cast<size_t>(width * height * 3 / 2));
    AMediaFormat *cf = AMediaFormat_new();
    if (HW_ANDROID_AVC == codecId) {
        AMediaFormat_setString(cf, AMEDIAFORMAT_KEY_MIME, "video/avc");
        AMediaFormat_setInt32(cf, AMEDIAFORMAT_KEY_WIDTH, width);
        AMediaFormat_setInt32(cf, AMEDIAFORMAT_KEY_HEIGHT, height);
        if (HwFrameFormat::HW_IMAGE_YV12 == (HwFrameFormat) format->getInt32(KEY_FORMAT)) {
            AMediaFormat_setInt32(cf, AMEDIAFORMAT_KEY_COLOR_FORMAT, COLOR_FormatYUV420Flexible);
        }
        AMediaFormat_setInt32(cf, AMEDIAFORMAT_KEY_BIT_RATE,
                              (int32_t) format->getInt64(KEY_BIT_RATE));
        AMediaFormat_setInt32(cf, AMEDIAFORMAT_KEY_FRAME_RATE, format->getInt32(KEY_FPS));
        AMediaFormat_setInt32(cf, AMEDIAFORMAT_KEY_I_FRAME_INTERVAL, 10);
    }
    const char *mime;
    AMediaFormat_getString(cf, AMEDIAFORMAT_KEY_MIME, &mime);
    codec = AMediaCodec_createEncoderByType(mime);
    if (!codec) {
        Logcat::e("HWVC", "HwAndroidCodec::configure codec alloc failed");
        AMediaFormat_delete(cf);
        return false;
    }
    media_status_t ret = AMediaCodec_configure(codec, cf, nullptr, nullptr,
                                               AMEDIACODEC_CONFIGURE_FLAG_ENCODE);
    if (media_status_t::AMEDIA_OK != ret) {
        Logcat::e("HWVC", "HwAndroidCodec::configure codec configure failed(%d)", ret);
        AMediaFormat_delete(cf);
        return false;
    }
    ret = AMediaCodec_start(codec);
    if (media_status_t::AMEDIA_OK != ret) {
        Logcat::e("HWVC", "HwAndroidCodec::configure codec start failed(%d)", ret);
        AMediaFormat_delete(cf);
        return false;
    }
    AMediaFormat_delete(cf);
    AVPacket *pkt = nullptr;
    HwVideoFrame *frame = new HwVideoFrame(nullptr, HwFrameFormat::HW_IMAGE_YV12, width, height);
    frame->setPts(0);
    if (Hw::SUCCESS != push(frame)) {
        return Hw::FAILED;
    }
    for (int i = 0; i < 60; ++i) {
        pop(5000);
        if (buffers[0] && buffers[1]) {
            break;
        }
    }
    delete frame;
    if (HW_ANDROID_AVC == codecId) {
        if (!buffers[0] || !buffers[1]) {
            return Hw::FAILED;
        }
    }
    avPacket = av_packet_alloc();
    return Hw::SUCCESS;
}

HwResult HwAndroidCodec::start() {
    return Hw::SUCCESS;
}

HwResult HwAndroidCodec::encode(HwAbsMediaFrame *frame, void **packet) {
    if (Hw::SUCCESS == push(frame)) {
        if (Hw::SUCCESS == pop(2000)) {
            *packet = avPacket;
            return Hw::SUCCESS;
        }
    }
    return Hw::FAILED;
}

int32_t HwAndroidCodec::type() {
    switch (codecId) {
        case HW_ANDROID_AVC: {
            return 0;
        }
        default: {
            return 0;
        }
    }
}

HwBuffer *HwAndroidCodec::getExtraBuffer(string key) {
    if ("csd-0" == key) {
        return buffers[0];
    } else if ("csd-1" == key) {
        return buffers[1];
    } else if ("csd-3" == key) {
        return buffers[2];
    } else if ("csd-4" == key) {
        return buffers[3];
    }
    return nullptr;
}

HwResult HwAndroidCodec::push(HwAbsMediaFrame *frame) {
    ssize_t bufIdx = AMediaCodec_dequeueInputBuffer(codec, 2000);
    if (bufIdx >= 0) {
        if (!frame) {
            media_status_t result = AMediaCodec_queueInputBuffer(codec, bufIdx, 0, 0, 0,
                                                                 AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM);
            if (media_status_t::AMEDIA_OK != result) {
                Logcat::e("HWVC", "HwAndroidCodec::push queue eof buffer failed(%d)", result);
            }
            return Hw::SUCCESS;
        }
        size_t bufSize = 0;
        int64_t pts = frame->getPts();
        auto buf = AMediaCodec_getInputBuffer(codec, bufIdx, &bufSize);
        if (bufSize > 0) {
            memcpy(buf, frame->getBuffer()->getData(), frame->getBufferSize());
        }
        media_status_t result = AMediaCodec_queueInputBuffer(codec, bufIdx,
                                                             0, bufSize,
                                                             pts, 0);
        if (media_status_t::AMEDIA_OK != result) {
            Logcat::e("HWVC", "HwAndroidCodec::push queue input buffer failed(%d)", result);
        }
        return Hw::SUCCESS;
    }
    Logcat::e("HWVC", "HwAndroidCodec::push failed bufIdx = %d", bufIdx);
    return Hw::FAILED;
}

HwResult HwAndroidCodec::pop(int32_t waitInUS) {
    AMediaCodecBufferInfo info;
    ssize_t bufIdx = AMediaCodec_dequeueOutputBuffer(codec, &info, waitInUS);
    switch (bufIdx) {
        case AMEDIACODEC_INFO_OUTPUT_BUFFERS_CHANGED: {
            Logcat::i("HWVC", "HwAndroidCodec AMEDIACODEC_INFO_OUTPUT_BUFFERS_CHANGED");
            break;
        }
        case AMEDIACODEC_INFO_OUTPUT_FORMAT_CHANGED: {
            Logcat::i("HWVC", "HwAndroidCodec AMEDIACODEC_INFO_OUTPUT_FORMAT_CHANGED");
            auto *format = AMediaCodec_getOutputFormat(codec);
            uint8_t *sps = nullptr;
            size_t spsSize = 0;
            uint8_t *pps = nullptr;
            size_t ppsSize = 0;
            AMediaFormat_getBuffer(format, "csd-0", reinterpret_cast<void **>(&sps), &spsSize);
            AMediaFormat_getBuffer(format, "csd-1", reinterpret_cast<void **>(&pps), &ppsSize);
            buffers[0] = HwBuffer::alloc(spsSize);
            buffers[1] = HwBuffer::alloc(ppsSize);
            memcpy(buffers[0]->getData(), sps, spsSize);
            memcpy(buffers[1]->getData(), pps, ppsSize);
            break;
        }
        case AMEDIACODEC_INFO_TRY_AGAIN_LATER: {
            Logcat::i("HWVC", "HwAndroidCodec AMEDIACODEC_INFO_TRY_AGAIN_LATER");
            break;
        }
        case AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM: {
            Logcat::i("HWVC", "HwAndroidCodec AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM");
            break;
        }
        default: {
            if (bufIdx < 0) break;
            size_t inBufSize = 0;
            auto buf = AMediaCodec_getOutputBuffer(codec, bufIdx, &inBufSize);
            if (buf) {
                bool wrote = false;
                auto endOfStream = info.flags & AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM;
                if (endOfStream == 0) {//如果没有收到BUFFER_FLAG_END_OF_STREAM信号，则代表输出数据时有效的
                    if (info.size > 0) {
                        if (info.flags & BUFFER_FLAG_CODEC_CONFIG) {//  config
                            if (configBuf && configBuf->size() != info.size) {
                                delete configBuf;
                                configBuf = nullptr;
                            }
                            if (!configBuf) {
                                configBuf = HwBuffer::alloc(info.size);
                            }
                            memcpy(configBuf->getData(), buf, configBuf->size());
                            wrote = false;
                        } else {
                            if (avPacket) {
                                av_packet_unref(avPacket);
                            }
                            av_init_packet(avPacket);
                            avPacket->pts = info.presentationTimeUs;
                            avPacket->dts = avPacket->pts;
                            if (info.flags & BUFFER_FLAG_KEY_FRAME) {// key frame
                                memcpy(keyFrameBuf->getData(),
                                       configBuf->getData(),
                                       configBuf->size());
                                memcpy(keyFrameBuf->getData() + configBuf->size(), buf, info.size);
                                avPacket->data = keyFrameBuf->getData();
                                avPacket->size = configBuf->size() + info.size;
                            } else {
                                avPacket->data = buf;
                                avPacket->size = info.size;
                            }
                            wrote = true;
                        }
                    }
                }
                //缓冲区使用完后必须把它还给MediaCodec，以便再次使用，至此一个流程结束，再次循环
                AMediaCodec_releaseOutputBuffer(codec, bufIdx, info.size != 0);
                return wrote ? Hw::SUCCESS : Hw::FAILED;
            }
        }
    }
    return Hw::FAILED;
}
