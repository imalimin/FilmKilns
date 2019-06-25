/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "../include/AsynVideoDecoder.h"
#include "TimeUtils.h"

AsynVideoDecoder::AsynVideoDecoder() : AbsAudioDecoder(), AbsVideoDecoder() {
    hwFrameAllocator = new HwFrameAllocator();
    decoder = new DefaultVideoDecoder();
}

AsynVideoDecoder::~AsynVideoDecoder() {
    stop();
    if (pipeline) {
        delete pipeline;
        pipeline = nullptr;
    }
    releaseLock.lock();
    if (decoder) {
        delete decoder;
        decoder = nullptr;
    }
    if (hwFrameAllocator) {
        delete hwFrameAllocator;
        hwFrameAllocator = nullptr;
    }
    releaseLock.unlock();
}

bool AsynVideoDecoder::prepare(string path) {
    playState = PAUSE;
    if (!pipeline) {
        pipeline = new EventPipeline("AsynVideoDecoder");
    }
    if (decoder) {
        if (!decoder->prepare(path)) {
            Logcat::e("HWVC", "AsynVideoDecoder::prepare failed");
            return false;
        }
    }
    return true;
}

bool AsynVideoDecoder::grab() {
    if (cache.size() >= 10) {
        grabLock.wait();
        return true;
    }
//    Logcat::i("HWVC", "HwFrameAllocator::grab  cache %d", cache.size());
    HwAbsMediaFrame *frame = nullptr;
    int ret = MEDIA_TYPE_UNKNOWN;
    releaseLock.lock();
    if (decoder) {
        ret = decoder->grab(&frame);
    }
    if (hwFrameAllocator && frame) {
        frame = hwFrameAllocator->ref(frame);
        cache.push(frame);
    }
    releaseLock.unlock();
    return MEDIA_TYPE_EOF != ret;
}

int AsynVideoDecoder::grab(HwAbsMediaFrame **frame) {
    if (PLAYING != playState || cache.empty()) {
        return MEDIA_TYPE_UNKNOWN;
    }
    if (outputFrame) {
        outputFrame->recycle();
    }
    hwFrameAllocator->printInfo();
    outputFrame = cache.front();
    cache.pop();
    grabLock.notify();
    *frame = outputFrame;
    if ((*frame)->isAudio()) {
        return MEDIA_TYPE_AUDIO;
    }
    return MEDIA_TYPE_VIDEO;
}

int AsynVideoDecoder::width() {
    if (decoder) {
        return decoder->width();
    }
    return 0;
}

int AsynVideoDecoder::height() {
    if (decoder) {
        return decoder->height();
    }
    return 0;
}

void AsynVideoDecoder::loop() {
    if (PLAYING != playState || !pipeline) {
        Logcat::i("HWVC", "AsynVideoDecoder::loop skip loop");
        return;
    }
    pipeline->queueEvent([this] {
        if (!grab()) {
            pause();
            Logcat::i("HWVC", "AsynVideoDecoder::loop EOF");
            return;
        }
        loop();
    });
}

void AsynVideoDecoder::start() {
    if (STOP == playState || PLAYING == playState) {
        return;
    }
    playState = PLAYING;
    loop();
}

void AsynVideoDecoder::pause() {
    if (STOP != playState) {
        playState = PAUSE;
    }
}

void AsynVideoDecoder::stop() {
    if (STOP != playState) {
        playState = STOP;
    }
    grabLock.notify();
}

bool AsynVideoDecoder::grabAnVideoFrame() {
//    while (true) {
//        int ret = decoder->grab(cacheFrame);
//        if (MEDIA_TYPE_VIDEO == ret) {
//            vRecycler->offer(cacheFrame);
//            return true;
//        } else {
//            vRecycler->recycle(cacheFrame);
//            return false;
//        }
//    }
}

int AsynVideoDecoder::getChannels() {
    if (decoder) {
        return decoder->getChannels();
    }
    return 0;
}

int AsynVideoDecoder::getSampleHz() {
    if (decoder) {
        return decoder->getSampleHz();
    }
    return 0;
}

int AsynVideoDecoder::getSampleFormat() {
    if (decoder) {
        return decoder->getSampleFormat();
    }
    return AV_SAMPLE_FMT_NONE;
}

int AsynVideoDecoder::getSamplesPerBuffer() {
    if (decoder) {
        return decoder->getSamplesPerBuffer();
    }
    return 0;
}

void AsynVideoDecoder::seek(int64_t us) {
    if (!decoder) {
        return;
    }
    decoder->seek(us);
}

int64_t AsynVideoDecoder::getVideoDuration() {
    if (decoder) {
        decoder->getVideoDuration();
    }
    return 0;
}

int64_t AsynVideoDecoder::getAudioDuration() {
    if (decoder) {
        decoder->getAudioDuration();
    }
    return 0;
}