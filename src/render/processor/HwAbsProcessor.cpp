/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwAbsProcessor.h"
#include "../include/Unit.h"
#include "HwString.h"

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
    provider = nullptr;
}

void HwAbsProcessor::registerAnUnit(Unit *unit) {
    if (pipeline) {
//        if(!provider){
//            Logcat::e("HWVC", "HwAbsProcessor::registerAnUnit failed. You must create an pipeline model.");
//            return;
//        }
        unit->setModelProvider(provider);
        pipeline->registerAnUnit(unit);
    } else {
        Logcat::e("HWVC",
                  "HwAbsProcessor::registerAnUnit failed. You must call startPipeline first.");
    }
}

void HwAbsProcessor::postEvent(Message *msg) {
    if (pipeline) {
        pipeline->postEvent(msg);
    } else {
        Logcat::e("HWVC", "Please call startPipeline first.");
        Logcat::i("HWVC", "HwAbsProcessor skip message %p", msg);
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

void HwAbsProcessor::putInt32(string unit, string key, int32_t value) {
    Message *msg = new Message(HwModelProvider::EVENT_PUT_INT32, nullptr,
                               Message::QUEUE_MODE_FIRST_ALWAYS, nullptr);
    msg->arg1 = value;
    msg->obj = new HwString(unit + "_" + key);
    postEvent(msg);
}

void HwAbsProcessor::putInt64(string unit, string key, int64_t value) {
    Message *msg = new Message(HwModelProvider::EVENT_PUT_INT64, nullptr,
                               Message::QUEUE_MODE_FIRST_ALWAYS, nullptr);
    msg->arg2 = value;
    msg->obj = new HwString(unit + "_" + key);
    postEvent(msg);
}

void HwAbsProcessor::putString(string unit, string key, string value) {
    Message *msg = new Message(HwModelProvider::EVENT_PUT_STRING, nullptr,
                               Message::QUEUE_MODE_FIRST_ALWAYS, nullptr);
    msg->obj = new HwString(unit + "_" + key);
    postEvent(msg);
}