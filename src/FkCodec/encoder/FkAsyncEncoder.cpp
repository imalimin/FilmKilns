/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkAsyncEncoder.h"
#include "FkFFEncoder.h"

#define TAG "FkAsyncEncoder"

FK_IMPL_CLASS_TYPE(FkAsyncEncoder, FkObject)

FkAsyncEncoder::FkAsyncEncoder(const FkAbsVideoEncoder::Desc &desc) : FkAbsVideoEncoder(desc) {
    released = false;
    hwFrameAllocator = new FkFrameAllocator();
    encoder = new FkFFEncoder(desc);
    mThread = FkHandlerThread::create(TAG);
    mHandler = new FkHandler(mThread->getLooper(), [this](std::shared_ptr<FkMessage> msg) {
        this->_dispatch(msg);
    });
}

FkAsyncEncoder::~FkAsyncEncoder() {
    mThread->quitSafely();
    mThread = nullptr;
    delete mHandler;
    mHandler = nullptr;
    if (encoder) {
        delete encoder;
        encoder = nullptr;
    }
    AlLogI(TAG, "size %d, %d, %d", aQueue.size(), vQueue.size(), tQueue.size());
    while (!aQueue.empty()) {
        aQueue.pop();
    }
    while (!vQueue.empty()) {
        vQueue.pop();
    }
    if (hwFrameAllocator) {
        delete hwFrameAllocator;
        hwFrameAllocator = nullptr;
    }
}

void FkAsyncEncoder::setBitrate(int32_t rate) {
    encoder->setBitrate(rate);
}

void FkAsyncEncoder::setProfile(std::string profile) {
    encoder->setProfile(profile);
}

void FkAsyncEncoder::setPreset(std::string preset) {
    encoder->setPreset(preset);
}

bool FkAsyncEncoder::prepare(std::string path, int width, int height, FkSampleFormat audioFormat) {
    if (encoder) {
        looping = encoder->prepare(path, width, height, audioFormat);
        loop();
        return looping;
    }
    return false;
}

FkResult FkAsyncEncoder::write(FkAbsMediaFrame *frame) {
    if (released) {
        return FK_FAIL;
    }
    if (vQueue.size() >= MAX_V_FRAME_CACHE && frame->isVideo()) {
        FkLogE(TAG, "Lack of cache, vQueue(%d), aQueue(%d), Skip Video Frame: pts=%"
                PRId64, vQueue.size(), aQueue.size(), frame->getPts());
        return FK_FAIL;
    }
    FkAbsMediaFrame *temp = hwFrameAllocator->ref(frame);
    if (temp) {
        {
            if (temp->isAudio()) {
                aQueue.push(temp);
            } else {
                vQueue.push(temp);
            }
            tQueue.push(temp->isAudio());
        }
        writeBlock.notify();
        return FK_OK;
    }
    return FK_FAIL;
}

void FkAsyncEncoder::write() {
    while (tQueue.empty()) {
        writeBlock.wait();
        if (!looping) {
            return;
        }
    }
    if (looping && encoder && !aQueue.empty() && !vQueue.empty() && !tQueue.empty()) {
        FkAbsMediaFrame *frame = nullptr;
        if (tQueue.pop()) {
            frame = aQueue.pop();
        } else {
            frame = vQueue.pop();
        }
        if (frame) {
            encoder->write(frame);
            frame->recycle();
        }
    }
}

FkResult FkAsyncEncoder::_dispatch(std::shared_ptr<FkMessage> &msg) {
    this->write();
    this->loop();
    return FK_OK;
}

void FkAsyncEncoder::loop() {
    {
        std::lock_guard<std::mutex> guard(mtx);
        if (!looping) {
            return;
        }
    }
    if (!mHandler)
        return;
    auto msg = FkMessage::obtain(0);
    mHandler->sendMessage(msg);
}

bool FkAsyncEncoder::stop() {
    bool ret = false;
    {
        std::lock_guard<std::mutex> guard(mtx);
        looping = false;
        if (encoder) {
            ret = encoder->stop();
        }
    }
    writeBlock.notify();
    return ret;
}

void FkAsyncEncoder::release() {
    released = true;
    if (encoder) {
        return encoder->release();
    }
}
