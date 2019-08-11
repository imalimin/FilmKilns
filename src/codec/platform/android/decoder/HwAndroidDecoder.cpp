/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include <libyuv.h>
#include "HwAndroidDecoder.h"
#include "BinaryUtils.h"
#include "Logcat.h"
#include "../../../include/HwVideoFrame.h"

HwAndroidDecoder::HwAndroidDecoder() : AbsAudioDecoder(), AbsVideoDecoder() {
}

HwAndroidDecoder::~HwAndroidDecoder() {
    if (codec) {
//        flush();
        AMediaCodec_stop(codec);
        AMediaCodec_delete(codec);
        codec = nullptr;
    }
    if (extractor) {
        AMediaExtractor_delete(extractor);
        extractor = nullptr;
    }
    if (outFrame) {
        delete outFrame;
        outFrame = nullptr;
    }
    if (vFmt) {
        AMediaFormat_delete(vFmt);
        vFmt = nullptr;
    }
}

bool HwAndroidDecoder::prepare(string path) {
    this->path = path;
    extractor = AMediaExtractor_new();
    media_status_t ret = AMediaExtractor_setDataSource(extractor, path.c_str());
    //打开输入视频文件
    if (AMEDIA_OK != ret) {
        Logcat::e("hwvc", "Couldn't open input stream.");
        return false;
    }
    int32_t count = AMediaExtractor_getTrackCount(extractor);
    if (count <= 0) {
        Logcat::e("hwvc", "HwAndroidDecoder::prepare couldn't find any track.");
        return false;
    }
    for (int i = 0; i < count; ++i) {
        AMediaFormat *format = AMediaExtractor_getTrackFormat(extractor, i);
        const char *mime;
        if (!AMediaFormat_getString(format, AMEDIAFORMAT_KEY_MIME, &mime)) {
            continue;
        } else if (!strncmp(mime, "video/", 6)) {
            vFmt = format;
            AMediaExtractor_selectTrack(extractor, i);
            break;
        }
    }
    return configure();
}

bool HwAndroidDecoder::configure() {
    AMediaFormat_getInt32(vFmt, AMEDIAFORMAT_KEY_WIDTH, &mWidth);
    AMediaFormat_getInt32(vFmt, AMEDIAFORMAT_KEY_HEIGHT, &mHeight);
    AMediaFormat_getInt64(vFmt, AMEDIAFORMAT_KEY_DURATION, &durationUs);
    const char *mime = nullptr;
    AMediaFormat_getString(vFmt, AMEDIAFORMAT_KEY_MIME, &mime);
    codec = AMediaCodec_createDecoderByType(mime);
    if (!codec) {
        Logcat::e("HWVC", "HwAndroidDecoder::configure codec alloc failed");
        return false;
    }
    media_status_t ret = AMediaCodec_configure(codec, vFmt, nullptr, nullptr, 0);
    if (media_status_t::AMEDIA_OK != ret) {
        Logcat::e("HWVC", "HwAndroidDecoder::configure codec configure failed(%d)", ret);
        return false;
    }
    ret = AMediaCodec_start(codec);
    if (media_status_t::AMEDIA_OK != ret) {
        Logcat::e("HWVC", "HwAndroidDecoder::configure codec start failed(%d)", ret);
        return false;
    }
    return true;
}

HwResult HwAndroidDecoder::push() {
    ssize_t bufIdx = AMediaCodec_dequeueInputBuffer(codec, 2000);
    if (bufIdx >= 0) {
        size_t bufSize = 0;
        int64_t pts = 0;
        auto buf = AMediaCodec_getInputBuffer(codec, bufIdx, &bufSize);
        if (bufSize > 0) {
            bufSize = AMediaExtractor_readSampleData(extractor, buf, bufSize);
            pts = AMediaExtractor_getSampleTime(extractor);
        }
        if (bufSize <= 0) {
            media_status_t result = AMediaCodec_queueInputBuffer(codec, bufIdx, 0, 0, 0,
                                                                 AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM);
            if (media_status_t::AMEDIA_OK != result) {
                Logcat::e("HWVC", "HwAndroidDecoder::push queue eof buffer failed(%d)", result);
            }
            return Hw::MEDIA_EOF;
        } else {
            media_status_t result = AMediaCodec_queueInputBuffer(codec, bufIdx,
                                                                 0, bufSize,
                                                                 pts, 0);
            if (media_status_t::AMEDIA_OK != result) {
                Logcat::e("HWVC", "HwAndroidDecoder::push queue input buffer failed(%d)", result);
            }
            AMediaExtractor_advance(extractor);
            return Hw::SUCCESS;
        }
    }
    Logcat::e("HWVC", "HwAndroidDecoder::push failed bufIdx = %d", bufIdx);
    return Hw::FAILED;
}

HwResult HwAndroidDecoder::pop(int32_t waitInUS) {
    AMediaCodecBufferInfo info;
    ssize_t bufIdx = AMediaCodec_dequeueOutputBuffer(codec, &info, waitInUS);
    switch (bufIdx) {
        case AMEDIACODEC_INFO_OUTPUT_BUFFERS_CHANGED: {
            Logcat::i("HWVC", "HwAndroidDecoder AMEDIACODEC_INFO_OUTPUT_BUFFERS_CHANGED");
            break;
        }
        case AMEDIACODEC_INFO_OUTPUT_FORMAT_CHANGED: {
            Logcat::i("HWVC", "HwAndroidDecoder AMEDIACODEC_INFO_OUTPUT_FORMAT_CHANGED");
            auto *format = AMediaCodec_getOutputFormat(codec);
            AMediaFormat_getInt32(format, "stride", &stride);
            int32_t color = 0;
            AMediaFormat_getInt32(format, AMEDIAFORMAT_KEY_COLOR_FORMAT, &color);
            HwFrameFormat colorFmt;
            if (COLOR_FormatYUV420SemiPlanar == color) {
                colorFmt = HwFrameFormat::HW_IMAGE_NV12;
            } else {
                colorFmt = HwFrameFormat::HW_IMAGE_YV12;
            }
            outFrame = new HwVideoFrame(nullptr, colorFmt, width(), height());
//            uint8_t *sps = nullptr;
//            size_t spsSize = 0;
//            uint8_t *pps = nullptr;
//            size_t ppsSize = 0;
//            AMediaFormat_getBuffer(format, "csd-0", reinterpret_cast<void **>(&sps), &spsSize);
//            AMediaFormat_getBuffer(format, "csd-1", reinterpret_cast<void **>(&pps), &ppsSize);
//            buffers[0] = HwBuffer::alloc(spsSize);
//            buffers[1] = HwBuffer::alloc(ppsSize);
//            memcpy(buffers[0]->getData(), sps, spsSize);
//            memcpy(buffers[1]->getData(), pps, ppsSize);
            break;
        }
        case AMEDIACODEC_INFO_TRY_AGAIN_LATER: {
            Logcat::i("HWVC", "HwAndroidDecoder AMEDIACODEC_INFO_TRY_AGAIN_LATER");
            return Hw::MEDIA_WAIT;
        }
        case AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM: {
            Logcat::i("HWVC", "HwAndroidDecoder AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM");
            return Hw::MEDIA_EOF;
        }
        default: {
            if (bufIdx < 0) break;
            size_t bufSize = 0;
            auto buf = AMediaCodec_getOutputBuffer(codec, bufIdx, &bufSize);
            if (buf) {
                bool wrote = false;
                auto endOfStream = info.flags & AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM;
                if (endOfStream == 0) {//如果没有收到BUFFER_FLAG_END_OF_STREAM信号，则代表输出数据时有效的
                    if (info.size > 0) {
                        if (info.flags & BUFFER_FLAG_CODEC_CONFIG) {//  config
//                            if (configBuf && configBuf->size() != info.size) {
//                                delete configBuf;
//                                configBuf = nullptr;
//                            }
//                            if (!configBuf) {
//                                configBuf = HwBuffer::alloc(info.size);
//                            }
//                            memcpy(configBuf->getData(), buf, configBuf->size());
                            wrote = false;
                        } else {
                            HwVideoFrame *videoFrame = dynamic_cast<HwVideoFrame *>(outFrame);
                            videoFrame->setPts(info.presentationTimeUs);
                            int32_t w = videoFrame->getWidth();
                            int32_t h = videoFrame->getHeight();
                            if (HwFrameFormat::HW_IMAGE_NV12 == videoFrame->getFormat()) {
                                int pixelCount = w * h;
                                libyuv::NV12ToI420(buf, stride,
                                                   buf + stride * h, stride,
                                                   videoFrame->getBuffer()->getData(),
                                                   videoFrame->getWidth(),
                                                   videoFrame->getBuffer()->getData() + pixelCount,
                                                   videoFrame->getWidth() / 2,
                                                   videoFrame->getBuffer()->getData() +
                                                   pixelCount * 5 / 4,
                                                   videoFrame->getWidth() / 2,
                                                   videoFrame->getWidth(), videoFrame->getHeight());
                            } else {
                                int32_t lines = h * 3 / 2;
                                for (int i = 0; i < lines; ++i) {
                                    memcpy(videoFrame->getBuffer()->getData() + i * w,
                                           buf + info.offset + i * stride * 3 / 2, w);
                                }
                            }
                            wrote = true;
                        }
                    }
                }
                //缓冲区使用完后必须把它还给MediaCodec，以便再次使用，至此一个流程结束，再次循环
                AMediaCodec_releaseOutputBuffer(codec, bufIdx, false);
                return wrote ? Hw::SUCCESS : Hw::FAILED;
            }
        }
    }
    return Hw::FAILED;
}

HwResult HwAndroidDecoder::grab(HwAbsMediaFrame **frame) {
    while (true) {
        HwResult ret = push();
        if (Hw::MEDIA_EOF == ret) {
            eof = true;
        }
        if (Hw::SUCCESS == pop(2000)) {
            *frame = outFrame;
            return Hw::MEDIA_SUCCESS;
        }
        //如果缓冲区中既没有音频也没有视频，并且已经读取完文件，则播放完了
        if (eof) {
            Logcat::i("HWVC", "HwAndroidDecoder::grab end");
            return Hw::MEDIA_EOF;
        }
    }
}

int HwAndroidDecoder::width() {
    return mWidth;
}

int HwAndroidDecoder::height() {
    return mHeight;
}

int HwAndroidDecoder::getChannels() {
    if (!aFmt) return 0;
    return 0;
}

int HwAndroidDecoder::getSampleHz() {
    if (!aFmt) return 0;
    return 0;
}

int HwAndroidDecoder::getSampleFormat() {
    return 0;
}

int HwAndroidDecoder::getSamplesPerBuffer() {
    if (!aFmt) return 0;
    return 1024;
}

void HwAndroidDecoder::seek(int64_t us) {

}

int64_t HwAndroidDecoder::getVideoDuration() {
    if (videoDurationUs >= 0) {
        return videoDurationUs;
    }
    videoDurationUs = getDuration();
    return videoDurationUs;
}

int64_t HwAndroidDecoder::getAudioDuration() {
    if (!aFmt) {
        audioDurationUs = 0;
    }
    if (audioDurationUs >= 0) {
        return audioDurationUs;
    }
    audioDurationUs = getDuration();
    return audioDurationUs;
}

int64_t HwAndroidDecoder::getDuration() {
    return durationUs;
}

void HwAndroidDecoder::start() {

}

void HwAndroidDecoder::pause() {

}

void HwAndroidDecoder::stop() {

}
