/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "EventPipeline.h"
#include "AlRunnable.h"

EventPipeline::EventPipeline(string name) {
    this->mThread = AlHandlerThread::create(name);
    this->shouldQuitThread = true;
}

EventPipeline::EventPipeline(AlLooper *looper) {
    this->mLooper = looper;
    this->shouldQuitThread = false;
}

EventPipeline::~EventPipeline() {
    simpleLock.lock();
    if (shouldQuitThread && mThread) {
        delete mThread;
    }
    mThread = nullptr;
    mLooper = nullptr;
    simpleLock.unlock();
}

void EventPipeline::queueEvent(function<void()> event) {
    simpleLock.lock();
    if (mLooper) {
        mLooper->sendMessage(AlMessage::obtain(0, new AlRunnable([event](Object *o) {
            event();
        })));
    }
    simpleLock.unlock();
}

void EventPipeline::quit() {
    simpleLock.lock();
    if (mLooper) {
        mLooper->sendMessage(AlMessage::obtain(0, nullptr, AlMessage::QUEUE_MODE_CLEAR));
        if (shouldQuitThread) {
            delete mThread;
        }
    }
    mThread = nullptr;
    simpleLock.unlock();
}