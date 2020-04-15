/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwAsyncEncoder.h"
#include "../include/HwFFmpegEncoder.h"

#define TAG "HwAsyncEncoder"

HwAsyncEncoder::HwAsyncEncoder() : HwAbsVideoEncoder() {
    pipeline = AlEventPipeline::create("HwAsyncFFEncoder");
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

void HwAsyncEncoder::setBitrate(int32_t rate) {
    encoder->setBitrate(rate);
}

void HwAsyncEncoder::setProfile(std::string profile) {
    encoder->setProfile(profile);
}

void HwAsyncEncoder::setPreset(std::string preset) {
    encoder->setPreset(preset);
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
    if (vQueue.size() >= MAX_V_FRAME_CACHE && frame->isVideo()) {
        _dropFrame();
        AlLogE(TAG, "Lack of cache, vQueue(%d), aQueue(%d), Skip Video Frame: pts=%"
                PRId64, vQueue.size(), aQueue.size(), frame->getPts());
        return Hw::FAILED;
    }
    HwAbsMediaFrame *temp = hwFrameAllocator->ref(frame);
    if (temp) {
        if (temp->isAudio()) {
            aQueue.push(temp);
        } else {
            vQueue.push(temp);
        }
        tQueue.push(temp->isAudio());
        writeBlock.notify();
        return Hw::SUCCESS;
    }
    return Hw::FAILED;
}

void HwAsyncEncoder::write() {
    while (tQueue.empty()) {
        writeBlock.wait();
        if (!looping) {
            return;
        }
    }
    simpleLock.lock();
    if (looping && encoder) {
        HwAbsMediaFrame *frame = nullptr;
        if (tQueue.front()) {
            frame = aQueue.front();
            aQueue.pop();
        } else {
            frame = vQueue.front();
            vQueue.pop();
        }
        tQueue.pop();
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

void HwAsyncEncoder::_dropFrame() {
}
