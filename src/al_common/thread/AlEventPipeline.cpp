/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "AlEventPipeline.h"
#include "AlRunnable.h"

AlEventPipeline *AlEventPipeline::create(std::string name) {
    return new AlEventPipeline(name);
}

AlEventPipeline *AlEventPipeline::create(AlLooper *looper) {
    return new AlEventPipeline(looper);
}

AlEventPipeline::AlEventPipeline(std::string name) : Object() {
    mThread = AlHandlerThread::create(name);
    setup(mThread->getLooper());
}

void AlEventPipeline::sleep(int32_t us) {
    this_thread::sleep_for(chrono::nanoseconds(us * 1000));
}

AlEventPipeline::AlEventPipeline(AlLooper *looper) {
    setup(looper);
}

AlEventPipeline::~AlEventPipeline() {
    quit();
    if (mThread) {
        mThread->quitSafely();
    }
    delete mThread;
    mThread = nullptr;
    delete mHandler;
    mHandler = nullptr;
}

void AlEventPipeline::setup(AlLooper *looper) {
    exited = false;
    mHandler = new AlHandler(looper, [](AlMessage *msg) {
        if (msg->obj) {
            auto *run = msg->getObj<AlRunnable *>();
            if (run) {
                (*run)(nullptr);
            }
        }
    });
}

void AlEventPipeline::queueEvent(function<void()> func) {
    if (exited) {
        return;
    }
    if (mHandler) {
        mHandler->sendMessage(AlMessage::obtain(0, new AlRunnable([func](Object *o) {
            func();
        })));
    }
}

void AlEventPipeline::quit() {
    if (exited) {
        return;
    }
    exited = true;
    if (mHandler) {
        mHandler->sendMessage(AlMessage::obtain(0, nullptr, AlMessage::QUEUE_MODE_CLEAR));
    }
}
