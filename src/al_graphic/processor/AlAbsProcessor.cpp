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
    registerAnUnit(this);
}

AlAbsProcessor::~AlAbsProcessor() {
}

void AlAbsProcessor::release() {
    release(AlRunnable::runEmptyArgs([]() {}));
}

void AlAbsProcessor::release(AlRunnable *runnable) {
    setOnDestroyListener(runnable);
    delete pipeline;
    pipeline = nullptr;
}

void AlAbsProcessor::onCreate() {
    AlLogI(TAG, "");
}

void AlAbsProcessor::onDestroy() {
    AlLogI(TAG, "");
    if (destroyRun) {
        (*destroyRun)(nullptr);
        delete destroyRun;
    }
}

void AlAbsProcessor::registerAnUnit(Unit *unit) {
    if (pipeline) {
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

void AlAbsProcessor::setOnDestroyListener(AlRunnable *runnable) {
    this->destroyRun = runnable;
}