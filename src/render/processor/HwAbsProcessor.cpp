/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwAbsProcessor.h"

HwAbsProcessor::HwAbsProcessor(string name) : Object(), name(name) {
}

HwAbsProcessor::~HwAbsProcessor() {

}

void HwAbsProcessor::startPipeline() {
    if (!pipeline) {
        pipeline = new UnitPipeline(name);
    }
}

void HwAbsProcessor::stopPipeline() {
    if (pipeline) {
        pipeline->release();
        delete pipeline;
        pipeline = nullptr;
    }
}

void HwAbsProcessor::registerAnUnit(Unit *unit) {
    if (pipeline) {
        pipeline->registerAnUnit(unit);
    } else {
        Logcat::e("HWVC", "Please call startPipeline first.");
    }
}

void HwAbsProcessor::postEvent(Message *msg) {
    if (pipeline) {
        pipeline->postEvent(msg);
    } else {
        Logcat::e("HWVC", "Please call startPipeline first.");
    }
}

void HwAbsProcessor::postEventAtFront(Message *msg) {
    if (pipeline) {
        pipeline->postEventAtFront(msg);
    } else {
        Logcat::e("HWVC", "Please call startPipeline first.");
    }
}

void HwAbsProcessor::removeAllMessage(int what) {
    if (pipeline) {
        pipeline->removeAllMessage(what);
    } else {
        Logcat::e("HWVC", "Please call startPipeline first.");
    }
}

void HwAbsProcessor::post(function<void()> runnable) {
    if (runnable) {
        Message *msg = new Message(0, [runnable](Message *msg2) {
            msg2->runnable = nullptr;
            runnable();
        });
        postEvent(msg);
    }
}