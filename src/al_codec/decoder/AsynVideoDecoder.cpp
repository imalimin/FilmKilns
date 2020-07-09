/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "AsynVideoDecoder.h"
#include "TimeUtils.h"
#include "DefaultVideoDecoder.h"
#include "AlPicFrameDecoder.h"
#include "../platform/android/decoder/HwAndroidDecoder.h"

AsynVideoDecoder::AsynVideoDecoder(bool onlyPic) : AbsVideoDecoder() {
    playing = false;
    hwFrameAllocator = new HwFrameAllocator();
    if (onlyPic) {
        decoder = new AlPicFrameDecoder();
    } else {
        decoder = new DefaultVideoDecoder();
    }
}

AsynVideoDecoder::~AsynVideoDecoder() {
    stop();
    if (pipeline) {
        pipeline->quit();
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
    playing = false;
    if (!pipeline) {
        pipeline = AlEventPipeline::create("AsynVideoDecoder");
    }
    if (decoder) {
        if (!decoder->prepare(path)) {
            Logcat::e("HWVC", "AsynVideoDecoder::prepare failed");
            delete decoder;
            decoder = nullptr;
            return false;
        }
    }
    return true;
}

void AsynVideoDecoder::clear() {
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

bool AsynVideoDecoder::grab() {
    if (cache.size() >= MAX_FRAME_CACHE) {
        grabLock.wait();
        return true;
    }
//    Logcat::i("HWVC", "HwFrameAllocator::grab  cache %d", cache.size());
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

HwResult AsynVideoDecoder::grab(HwAbsMediaFrame **frame) {
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
    }
    hwFrameAllocator->printInfo();
    outputFrame = cache.front();
    cache.pop();
    grabLock.notify();
    *frame = outputFrame;
    return Hw::MEDIA_SUCCESS;
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
    if (!playing || !pipeline) {
        Logcat::i("HWVC", "AsynVideoDecoder::loop skip loop");
        return;
    }
    pipeline->queueEvent([this] {
        if (!grab()) {
            stop();
            Logcat::i("HWVC", "AsynVideoDecoder::loop EOF");
            return;
        }
        loop();
    });
}

void AsynVideoDecoder::start() {
    if (playing) {
        return;
    }
    playing = true;
    decoder->start();
    loop();
}

void AsynVideoDecoder::pause() {
    decoder->pause();
}

void AsynVideoDecoder::stop() {
    if (!playing) {
        return;
    }
    playing = false;
    decoder->stop();
    grabLock.notify();
}

void AsynVideoDecoder::setOutSampleFormat(HwSampleFormat format) {

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
    clear();
    decoder->seek(us);
}

int64_t AsynVideoDecoder::getVideoDuration() {
    if (decoder) {
        return decoder->getVideoDuration();
    }
    return -1;
}

int64_t AsynVideoDecoder::getAudioDuration() {
    if (decoder) {
        return decoder->getAudioDuration();
    }
    return -1;
}

int64_t AsynVideoDecoder::getDuration() {
    if (decoder) {
        return decoder->getDuration();
    }
    return -1;
}

int64_t AsynVideoDecoder::getAudioStartTime() {
    return 0;
}