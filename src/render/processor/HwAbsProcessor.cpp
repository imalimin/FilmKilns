/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwAbsProcessor.h"
#include "HwPair.h"

HwAbsProcessor::HwAbsProcessor(string name) : Object(), name(name) {
    pipeline = new UnitPipeline(name);
    provider = new HwModelProvider(ALIAS_OF_MODEL_PROVIDER);
    registerAnUnit(provider);
}

HwAbsProcessor::~HwAbsProcessor() {
    if (pipeline) {
        pipeline->release();
        delete pipeline;
        pipeline = nullptr;
    }
    provider = nullptr;
    onDestroy();
}

void HwAbsProcessor::onDestroy() {

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

HwPairBuilder<int32_t> HwAbsProcessor::putInt32(string key, int32_t value) {
    return HwPairBuilder<int32_t>(pipeline, HwModelProvider::EVENT_PUT_INT32,
                                  HwPair<string, int32_t>(key, value));
}

HwPairBuilder<int64_t> HwAbsProcessor::putInt64(string key, int64_t value) {
    return HwPairBuilder<int64_t>(pipeline, HwModelProvider::EVENT_PUT_INT64,
                                  HwPair<string, int64_t>(key, value));
}

HwPairBuilder<string> HwAbsProcessor::putString(string key, string value) {
    return HwPairBuilder<string>(pipeline, HwModelProvider::EVENT_PUT_STRING,
                                 HwPair<string, string>(key, value));
}

HwPairBuilder<Object *> HwAbsProcessor::putObject(string key, Object *value) {
    return HwPairBuilder<Object *>(pipeline, HwModelProvider::EVENT_PUT_OBJECT,
                                   HwPair<string, Object *>(key, value));
}