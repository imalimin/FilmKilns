/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlAbsProcessor.h"
#include "AlRunnable.h"
#include "HwPair.h"

#define TAG "AlAbsProcessor"

AlAbsProcessor::AlAbsProcessor(string name) : Unit(name, Unit::AlUnitSetting(true)), name(name) {
    pipeline = AlPostMan::create(name);
    provider = new HwModelProvider(ALIAS_OF_MODEL_PROVIDER);
    registerAnUnit(this);
    registerAnUnit(provider);
}

AlAbsProcessor::~AlAbsProcessor() {
}

void AlAbsProcessor::release() {
    delete pipeline;
    pipeline = nullptr;
    provider = nullptr;
}

void AlAbsProcessor::onCreate() {
    AlLogI(TAG, "");
}

void AlAbsProcessor::onDestroy() {
    AlLogI(TAG, "");
}

void AlAbsProcessor::registerAnUnit(Unit *unit) {
    if (pipeline) {
        unit->setModelProvider(provider);
        pipeline->registerAnUnit(unit);
    } else {
        AlLogI(TAG, "failed. You must call startPipeline first.");
    }
}

void AlAbsProcessor::post(function<void()> runnable) {
    if (runnable) {
        AlMessage *msg = AlMessage::obtain(0, new AlRunnable([runnable](Object *o) {
            runnable();
        }));
        postEvent(msg);
    }
}

HwPairBuilder<int32_t> AlAbsProcessor::putInt32(string key, int32_t value) {
    return HwPairBuilder<int32_t>(pipeline, HwModelProvider::EVENT_PUT_INT32,
                                  HwPair<string, int32_t>(key, value));
}

HwPairBuilder<int64_t> AlAbsProcessor::putInt64(string key, int64_t value) {
    return HwPairBuilder<int64_t>(pipeline, HwModelProvider::EVENT_PUT_INT64,
                                  HwPair<string, int64_t>(key, value));
}

HwPairBuilder<string> AlAbsProcessor::putString(string key, string value) {
    return HwPairBuilder<string>(pipeline, HwModelProvider::EVENT_PUT_STRING,
                                 HwPair<string, string>(key, value));
}

HwPairBuilder<Object *> AlAbsProcessor::putObject(string key, Object *value) {
    return HwPairBuilder<Object *>(pipeline, HwModelProvider::EVENT_PUT_OBJECT,
                                   HwPair<string, Object *>(key, value));
}

bool AlAbsProcessor::onCreate(AlMessage *msg) {
    AlLogI(TAG, "");
    onCreate();
    return true;
}

bool AlAbsProcessor::onDestroy(AlMessage *msg) {
    AlLogI(TAG, "");
    onDestroy();
    return true;
}
