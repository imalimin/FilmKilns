//
// Created by mingyi.li on 2018/12/25.
//

#include "UnitPipeline.h"
#include "AlRunnable.h"
#include "Unit.h"
#include "AlMessage.h"

UnitPipeline::UnitPipeline(string name) {
    notified = false;
    mThread = AlHandlerThread::create(name);
    mHandler = new AlHandler(mThread->getLooper(), [this](AlMessage *msg) {
        if (msg && msg->obj && typeid(AlRunnable) == typeid(*msg->obj)) {
            AlRunnable *run = dynamic_cast<AlRunnable *>(msg->obj);
            (*run)(msg);
        }
        this->dispatch(msg);
    });
}

UnitPipeline::~UnitPipeline() {
    postDestroy();
    mThread->quitSafely();
    delete mThread;
    mThread = nullptr;
    delete mHandler;
    mHandler = nullptr;
}

void UnitPipeline::postDestroy() {
    simpleLock.lock();
    if (!notified) {
        notified = true;
        if (mHandler) {
            AlMessage *msg = AlMessage::obtain(EVENT_COMMON_RELEASE);
            mHandler->sendMessage(AlMessage::obtain(EVENT_COMMON_RELEASE, msg));
        }
    }
    simpleLock.unlock();
}

void UnitPipeline::postEvent(AlMessage *msg) {
    simpleLock.lock();
    if (mHandler) {
        mHandler->sendMessage(msg);
        simpleLock.unlock();
        return;
    }
    simpleLock.unlock();
    this->dispatch(msg);
}

void UnitPipeline::dispatch(AlMessage *msg) {
    for (auto itr = units.cbegin(); itr != units.cend(); itr++) {
        bool ret = (*itr)->dispatch(msg);
    }
    if (EVENT_COMMON_RELEASE == msg->what) {
        clear();
    }
}

void UnitPipeline::clear() {
    LOGI("UnitPipeline::clear units");
    for (auto unit:units) {
        delete unit;
    }
    units.clear();
}

int UnitPipeline::registerAnUnit(Unit *unit) {
    unit->setController(this);
    units.push_back(unit);
    return 1;
}
