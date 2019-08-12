/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwAsyncEncoder.h"
#include "../include/HwFFmpegEncoder.h"

HwAsyncEncoder::HwAsyncEncoder() : HwAbsVideoEncoder() {
    pipeline = new EventPipeline("HwAsyncFFEncoder");
    hwFrameAllocator = new HwFrameAllocator();
    encoder = new HwFFmpegEncoder();
}

HwAsyncEncoder::~HwAsyncEncoder() {
    if (encoder) {
        delete encoder;
        encoder = nullptr;
    }
    if (hwFrameAllocator) {
        delete hwFrameAllocator;
        hwFrameAllocator = nullptr;
    }
    if (pipeline) {
        delete pipeline;
        pipeline = nullptr;
    }
}

bool HwAsyncEncoder::prepare(string path, int width, int height, HwSampleFormat audioFormat) {
    if (encoder) {
        looping = encoder->prepare(path, width, height, audioFormat);
        loop();
        return looping;
    }
    return false;
}

HwResult HwAsyncEncoder::write(HwAbsMediaFrame *frame) {
    if (cache.size() >= MAX_FRAME_CACHE) {
        Logcat::e("HWVC", "HwAsyncFFEncoder::write lack of cache.");
        return Hw::FAILED;
    }
    HwAbsMediaFrame *temp = hwFrameAllocator->ref(frame);
    if (temp) {
        cache.push(temp);
        writeBlock.notify();
        return Hw::SUCCESS;
    }
    return Hw::FAILED;
}

void HwAsyncEncoder::write() {
    while (cache.empty()) {
        writeBlock.wait();
        if (!looping) {
            return;;
        }
    }
    simpleLock.lock();
    if (looping && encoder) {
        HwAbsMediaFrame *frame = cache.front();
        cache.pop();
        if (frame) {
            encoder->write(frame);
            frame->recycle();
        }
    }
    simpleLock.unlock();
}

void HwAsyncEncoder::loop() {
    simpleLock.lock();
    if (!looping) {
        simpleLock.unlock();
        return;
    }
    simpleLock.unlock();
    if (!pipeline)
        return;
    pipeline->queueEvent([this]() {
        this->write();
        this->loop();
    });
}

bool HwAsyncEncoder::stop() {
    simpleLock.lock();
    looping = false;
    bool ret = false;
    if (encoder) {
        ret = encoder->stop();
    }
    simpleLock.unlock();
    writeBlock.notify();
    return ret;
}

void HwAsyncEncoder::release() {
    if (encoder) {
        return encoder->release();
    }
}