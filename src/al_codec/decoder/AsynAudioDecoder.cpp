/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AsynAudioDecoder.h"
#include "TimeUtils.h"

#define TAG "AsynAudioDecoder"

AsynAudioDecoder::AsynAudioDecoder() : AbsAudioDecoder() {
    playing = false;
    hwFrameAllocator = new HwFrameAllocator();
    decoder = new DefaultAudioDecoder();
//    decoder = new HwAndroidAudioDecoder();
}

AsynAudioDecoder::~AsynAudioDecoder() {
    AlLogI(TAG, "0");
    stop();
    AlLogI(TAG, "1");
    if (pipeline) {
        delete pipeline;
        pipeline = nullptr;
    }
    AlLogI(TAG, "2");
    releaseLock.lock();
    if (decoder) {
        delete decoder;
        decoder = nullptr;
    }
    AlLogI(TAG, "3");
    if (hwFrameAllocator) {
        delete hwFrameAllocator;
        hwFrameAllocator = nullptr;
    }
    AlLogI(TAG, "4");
    releaseLock.unlock();
    AlLogI(TAG, "5");
}

bool AsynAudioDecoder::prepare(string path) {
    playing = false;
    if (!pipeline) {
        pipeline = AlEventPipeline::create("AsynAudioDecoder");
    }
    if (decoder) {
        if (!decoder->prepare(path)) {
            AlLogE(TAG, "failed");
            delete decoder;
            decoder = nullptr;
            return false;
        }
    }
//    HwAndroidAudioDecoder *d = new HwAndroidAudioDecoder();
//    d->prepare(path);
//    delete d;
//    start();
    return true;
}

void AsynAudioDecoder::seek(int64_t us) {
    if (!decoder) {
        return;
    }
    clear();
    decoder->seek(us);
}

void AsynAudioDecoder::start() {
    if (playing) {
        return;
    }
    playing = true;
    decoder->start();
    loop();
}

void AsynAudioDecoder::pause() {
    decoder->pause();
}

void AsynAudioDecoder::stop() {
    if (!playing) {
        return;
    }
    playing = false;
    decoder->stop();
    grabLock.notify();
}

void AsynAudioDecoder::loop() {
    if (!playing || !pipeline) {
        AlLogI(TAG, "skip loop");
        return;
    }
    pipeline->queueEvent([this] {
        if (!grab()) {
            stop();
            AlLogI(TAG, "loop EOF");
            return;
        }
        loop();
    });
}

void AsynAudioDecoder::clear() {
    releaseLock.lock();
    while (!cache.empty()) {
        outputFrame = cache.front();
        cache.pop();
        if (outputFrame) {
            outputFrame->recycle();
            outputFrame = nullptr;
        }
    }
    releaseLock.unlock();
    grabLock.notify();
}

bool AsynAudioDecoder::grab() {
    if (!decoder) {
        return false;
    }
    if (cache.size() >= 10) {
        grabLock.wait();
        return true;
    }
//    AlLogD(TAG, "cache %d", cache.size());
    HwAbsMediaFrame *frame = nullptr;
    HwResult ret = Hw::MEDIA_WAIT;
    releaseLock.lock();
    if (decoder) {
        ret = decoder->grab(&frame);
    }
    if (hwFrameAllocator && frame) {
        frame = hwFrameAllocator->ref(frame);
        cache.push(frame);
    }
    releaseLock.unlock();
    return Hw::MEDIA_EOF != ret;
}

HwResult AsynAudioDecoder::grab(HwAbsMediaFrame **frame) {
    if (cache.empty()) {
        /*
         * If none cache and playing is false, that mean decoder is eof.
         * Value of playing will be false when decoder is eof.
         */
        if (!playing) {
            return Hw::MEDIA_EOF;
        }
        return Hw::MEDIA_WAIT;
    }
    if (outputFrame) {
        outputFrame->recycle();
        outputFrame = nullptr;
    }
    hwFrameAllocator->printInfo();
    outputFrame = cache.front();
    cache.pop();
    grabLock.notify();
    *frame = outputFrame;
//    memset(outputFrame->getData(), 1, outputFrame->getDataSize());
    return Hw::SUCCESS;
}

int AsynAudioDecoder::getChannels() {
    if (decoder) {
        return decoder->getChannels();
    }
    return 0;
}

int AsynAudioDecoder::getSampleHz() {
    if (decoder) {
        return decoder->getSampleHz();
    }
    return 0;
}

int AsynAudioDecoder::getSampleFormat() {
    if (decoder) {
        return decoder->getSampleFormat();
    }
    return AV_SAMPLE_FMT_NONE;
}

int AsynAudioDecoder::getSamplesPerBuffer() {
    if (decoder) {
        return decoder->getSamplesPerBuffer();
    }
    return 0;
}

int64_t AsynAudioDecoder::getAudioDuration() {
    if (decoder) {
        return decoder->getAudioDuration();
    }
    return -1;
}

int64_t AsynAudioDecoder::getDuration() {
    if (decoder) {
        return decoder->getDuration();
    }
    return -1;
}