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
    pipeline = new UnitPipeline(name);
    provider = new HwModelProvider(ALIAS_OF_MODEL_PROVIDER);
    registerAnUnit(this);
    registerAnUnit(provider);
}

AlAbsProcessor::~AlAbsProcessor() {
}

void AlAbsProcessor::prepare() {
    if (pipeline) {
        post([=] {
            onCreate();
        });
    } else {
        onCreate();
    }
    AlMessage *msg = AlMessage::obtain(EVENT_COMMON_PREPARE);
    postEvent(msg);
}

void AlAbsProcessor::release() {
    if (pipeline) {
        pipeline->postDestroy();
        post([=] {
            onDestroy();
        });
        delete pipeline;
        pipeline = nullptr;
    } else {
        onDestroy();
    }
    provider = nullptr;
}

void AlAbsProcessor::onCreate() {
    Logcat::e(TAG, "%s(%d)", __FUNCTION__, __LINE__);
}

void AlAbsProcessor::onDestroy() {
    Logcat::e(TAG, "%s(%d)", __FUNCTION__, __LINE__);
}

void AlAbsProcessor::registerAnUnit(Unit *unit) {
    if (pipeline) {
        unit->setModelProvider(provider);
        pipeline->registerAnUnit(unit);
    } else {
        Logcat::e(TAG, "%s(%d) failed. You must call startPipeline first.", __FUNCTION__, __LINE__);
    }
}

void AlAbsProcessor::postEvent(AlMessage *msg) {
    if (pipeline) {
        pipeline->postEvent(msg);
    } else {
        Logcat::e(TAG, "%s(%d) failed. Please call startPipeline first.", __FUNCTION__, __LINE__);
        Logcat::i(TAG, "%s(%d) failed. AlAbsProcessor skip message %p",
                  __FUNCTION__, __LINE__, msg);
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
    return true;
}

bool AlAbsProcessor::onDestroy(AlMessage *msg) {
    return true;
}
