/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include <libyuv.h>
#include "HwAndroidDecoder.h"
#include "BinaryUtils.h"
#include "Logcat.h"
#include "../../../include/HwVideoFrame.h"
#include "../core/HwAndroidCodec.h"

HwAndroidDecoder::HwAndroidDecoder() : AbsAudioDecoder(), AbsVideoDecoder() {
}

HwAndroidDecoder::~HwAndroidDecoder() {
    if (codec) {
        delete codec;
        codec = nullptr;
    }
    if (extractor) {
        AMediaExtractor_delete(extractor);
        extractor = nullptr;
    }
    if (vSampleBuf) {
        delete vSampleBuf;
        vSampleBuf = nullptr;
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
    uint8_t *csd0 = nullptr, *csd1 = nullptr;
    size_t csd0Size = 0, csd1Size = 0;
    AMediaFormat_getInt32(vFmt, AMEDIAFORMAT_KEY_WIDTH, &mWidth);
    AMediaFormat_getInt32(vFmt, AMEDIAFORMAT_KEY_HEIGHT, &mHeight);
    AMediaFormat_getInt64(vFmt, AMEDIAFORMAT_KEY_DURATION, &durationUs);
    AMediaFormat_getBuffer(vFmt, "csd-0", reinterpret_cast<void **>(&csd0), &csd0Size);
    AMediaFormat_getBuffer(vFmt, "csd-1", reinterpret_cast<void **>(&csd1), &csd1Size);
    HwBuffer *csd0Buf = HwBuffer::wrap(csd0, csd0Size);
    HwBuffer *csd1Buf = HwBuffer::wrap(csd1, csd1Size);
    HwBundle bundle;
    bundle.putInt32(HwAbsCodec::KEY_WIDTH, mWidth);
    bundle.putInt32(HwAbsCodec::KEY_HEIGHT, mHeight);
    bundle.putObject("csd-0", csd0Buf);
    bundle.putObject("csd-1", csd1Buf);
    codec = HwAndroidCodec::createDecoder(HwAndroidCodec::HW_ANDROID_AVC);
    if (Hw::SUCCESS != codec->configure(bundle)) {
        delete csd0Buf;
        delete csd1Buf;
        Logcat::e("HWVC", "HwAndroidDecoder::configure failed to configure audio codec!");
        return false;
    }
    delete csd0Buf;
    delete csd1Buf;
    codec->start();
    vSampleBuf = HwBuffer::alloc(mWidth * mHeight * 3 / 2);
    return true;
}

void HwAndroidDecoder::handleAction() {
    if (actionSeekInUs >= 0) {
        if (extractor) {
            AMediaExtractor_seekTo(extractor, actionSeekInUs, AMEDIAEXTRACTOR_SEEK_CLOSEST_SYNC);
            if (codec) {
                codec->flush();
            }
        }
        Logcat::i("hwvc", "HwAndroidDecoder::handleAction seek to %lld", actionSeekInUs);
        actionSeekInUs = -1;
    }
}

HwResult HwAndroidDecoder::grab(HwAbsMediaFrame **frame) {
    handleAction();
    *frame = nullptr;
    if (eof) {
        Logcat::i("HWVC", "HwAndroidDecoder::grab end");
        return Hw::MEDIA_EOF;
    }
    while (true) {
        ssize_t bufSize = AMediaExtractor_readSampleData(extractor,
                                                         vSampleBuf->data(),
                                                         vSampleBuf->size());
        int64_t pts = AMediaExtractor_getSampleTime(extractor);
        if (bufSize > 0) {
            HwPacket *pkt = HwPacket::wrap(vSampleBuf->data(), bufSize, pts, 0);
            HwResult ret = codec->process(frame, &pkt);
            delete pkt;
            if (Hw::SUCCESS == ret) {
                AMediaExtractor_advance(extractor);
            }
            if (*frame) {
                return Hw::SUCCESS;
            }
        } else {
            // Notify codec finish.
            HwResult ret = codec->process(frame, nullptr);
            if (Hw::MEDIA_EOF == ret) {
                eof = true;
            }
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
    actionSeekInUs = us;
    eof = false;
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
