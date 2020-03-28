//
// Created by mingyi.li on 2018/12/25.
//

#include "UnitPipeline.h"
#include "AlRunnable.h"
#include "Unit.h"
#include "AlMessage.h"

#define TAG "UnitPipeline"

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
    std::lock_guard<std::mutex> guard(mtx);
    if (!notified) {
        notified = true;
        if (mHandler) {
            _notifyDestroy();
        }
    }
}

void UnitPipeline::postEvent(AlMessage *msg) {
    std::lock_guard<std::mutex> guard(mtx);
    if (mHandler) {
        mHandler->sendMessage(msg);
        return;
    }
    this->dispatch(msg);
}

void UnitPipeline::dispatch(AlMessage *msg) {
    if (EVENT_COMMON_RELEASE != msg->what) {
        for (auto itr = units.cbegin(); itr != units.cend(); itr++) {
            bool ret = (*itr)->dispatch(msg);
        }
    } else {
        for (auto itr = units.end() - 1; itr != units.begin() - 1; --itr) {
            auto *unit = *itr;
            Logcat::i(TAG, "%s(%d) %s will be destroy.",
                      __FUNCTION__, __LINE__,
                      typeid(*unit).name());
            bool ret = unit->dispatch(msg);
        }
        clear();
    }
}

void UnitPipeline::clear() {
    Logcat::i(TAG, "%s(%d)", __FUNCTION__, __LINE__);
    for (auto unit:units) {
        if (0 == unit->setting.hosted) {
            delete unit;
        }
    }
    units.clear();
}

int UnitPipeline::registerAnUnit(Unit *unit) {
    units.push_back(unit);
    _notifyCreate();
    return 1;
}

void UnitPipeline::_notifyCreate() {
    postEvent(AlMessage::obtain(EVENT_COMMON_PREPARE, nullptr, AlMessage::QUEUE_MODE_FIRST_ALWAYS));
}

void UnitPipeline::_notifyDestroy() {
    mHandler->sendMessage(AlMessage::obtain(EVENT_COMMON_RELEASE));
}
