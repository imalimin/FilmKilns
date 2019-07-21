/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwAbsProcessor.h"
#include "../include/Unit.h"

HwAbsProcessor::HwAbsProcessor(string name) : Object(), name(name) {
}

HwAbsProcessor::~HwAbsProcessor() {
    if (model) {
        delete model;
        model = nullptr;
    }
}

void HwAbsProcessor::startPipeline() {
    if (!pipeline) {
        pipeline = new UnitPipeline(name);
    }
    _createModel();
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
        if(!getModel()){
            Logcat::e("HWVC", "HwAbsProcessor::registerAnUnit failed. You must create an pipeline model.");
            return;
        }
        unit->setModel(getModel());
        pipeline->registerAnUnit(unit);
    } else {
        Logcat::e("HWVC", "HwAbsProcessor::registerAnUnit failed. You must call startPipeline first.");
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

HwAbsPipelineModel *HwAbsProcessor::getModel() {
    return model;
}

void HwAbsProcessor::_createModel() {
    model = createModel();
}