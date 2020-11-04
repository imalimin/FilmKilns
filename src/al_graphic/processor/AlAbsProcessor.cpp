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
    registerMonitor();
}

AlAbsProcessor::~AlAbsProcessor() {
}

void AlAbsProcessor::release() {
    release(this->destroyRun);
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

void AlAbsProcessor::registerMonitor() {
    auto s = Unit::AlUnitSetting(false);
    s.endNode = true;
    auto *u = new FkUMonitor("engine_monitor", s);
    u->setOnMonitorListener([this](int monitorState, std::shared_ptr<AlVector<std::shared_ptr<FkUnitDesc>>> unitsDesc,
                               std::shared_ptr<FkMsgState> state) {
        std::lock_guard<std::mutex> guard(this->monitorMtx);
        if (this->onMonitorListener) {
            this->onMonitorListener(monitorState, unitsDesc, state);
        }
    });
    registerAnUnit(u);
    al_reg_msg(MSG_MONITOR_NOTIFY, AlAbsProcessor::_onMonitorNotify);
}

bool AlAbsProcessor::_onMonitorNotify(AlMessage *msg) {
    return true;
}

void AlAbsProcessor::setOnMonitorListener(FkUMonitor::OnMonitorListener l) {
    std::lock_guard<std::mutex> guard(monitorMtx);
    onMonitorListener = l;
}
