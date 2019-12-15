//
// Created by mingyi.li on 2018/12/25.
//

#include "../include/UnitPipeline.h"
#include "../include/Unit.h"

UnitPipeline::UnitPipeline(string name) {
    available = true;
    mThread = AlHandlerThread::create(name);
    mHandler = new AlHandler(mThread->getLooper(), [this](AlMessage *msg) {
        Message *m = dynamic_cast<Message *>(msg->obj);
        if (m && m->runnable) {
            m->runnable(m);
        }
        this->dispatch(m);
    });
}

UnitPipeline::~UnitPipeline() {
    release();
    mThread->quitSafely();
    delete mThread;
    mThread = nullptr;
    delete mHandler;
    mHandler = nullptr;
}

void UnitPipeline::release() {
    if (available) {
        simpleLock.lock();
        available = false;
        Message *msg = new Message(EVENT_COMMON_RELEASE, nullptr);
        if (mHandler) {
            mHandler->sendMessage(AlMessage::obtain(EVENT_COMMON_RELEASE, msg));
        }
        simpleLock.unlock();
    }
}

void UnitPipeline::postEvent(Message *msg) {
    if (mHandler) {
        simpleLock.lock();
        if (available) {
            AlMessage *aMsg = AlMessage::obtain(msg->what, msg, msg->queueMode);
            aMsg->desc = msg->desc;
            mHandler->sendMessage(aMsg);
        } else {
            Logcat::i("HWVC", "UnitPipeline skip message %p", msg);
            delete msg;
        }
        simpleLock.unlock();
    } else {
        this->dispatch(msg);
    }
}

void UnitPipeline::dispatch(Message *msg) {
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
