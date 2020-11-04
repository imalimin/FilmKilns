//
// Created by mingyi.li on 2018/12/25.
//

#include "UnitPipeline.h"
#include "AlRunnable.h"
#include "Unit.h"
#include "AlMessage.h"
#include "StringUtils.h"
#include "TimeUtils.h"
#include "AlVector.h"
#include "FkUnitDesc.h"

#define TAG "UnitPipeline"
#define DEBUG_SHOW_COST 0

UnitPipeline::UnitPipeline(string name) {
    notified = false;
    mThread = AlHandlerThread::create(name);
    mHandler = new AlHandler(mThread->getLooper(), [this](AlMessage *msg) {
        if (msg && msg->obj && typeid(AlRunnable) == typeid(*msg->obj)) {
            AlRunnable *run = dynamic_cast<AlRunnable *>(msg->obj);
            (*run)(msg);
        }
        this->_dispatch(msg);
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
        msg->state.postTimeInUS = TimeUtils::getCurrentTimeUS();
        mHandler->sendMessage(msg);
    }
//    this->dispatch(msg);
}

void UnitPipeline::_dispatch(AlMessage *msg) {
#if 0
    AlLogI(TAG, "message(%s-%d), count of unit %d",
           kidRestore(msg->what).c_str(),
           msg->what, units.size());
#endif
    msg->state.handleTimeInUS = TimeUtils::getCurrentTimeUS();
    if (EVENT_COMMON_PREPARE == msg->what) {
        _disCreate(msg);
        return;
    }
    if (EVENT_COMMON_RELEASE == msg->what) {
        _disDestroy(msg);
        return;
    }
#if DEBUG_SHOW_COST
    auto time = TimeUtils::getCurrentTimeUS();
#endif
    std::vector<Unit *> endNodes;
    for (auto *unit:units) {
        // Handle msg on the end.
        if (unit->setting.endNode) {
            endNodes.emplace_back(unit);
            continue;
        }
        bool ret = unit->dispatch(msg);
    }
    for (auto *unit:endNodes) {
        bool ret = unit->dispatch(msg);
    }
#if DEBUG_SHOW_COST
    AlLogI(TAG, "%s cost %"
            PRId64,
           Unit::kidRestore(msg->what).c_str(), (TimeUtils::getCurrentTimeUS() - time) / 1000);
#endif
}

bool UnitPipeline::_disCreate(AlMessage *msg) {
//    std::lock_guard<std::mutex> guard(mtx);
    bool shouldNotifyDesc = false;
    while (!units0.empty()) {
        shouldNotifyDesc = true;
        auto *u = units0.front();
        AlLogI(TAG, "%s will be create.", u->toString().c_str());
        bool ret = u->dispatch(msg);
        units0.pop_front();
        units.emplace_back(u);
    }
    if (shouldNotifyDesc) {
        _notifyUnitsDesc();
    }
    return true;
}

bool UnitPipeline::_disDestroy(AlMessage *msg) {
    for (auto itr = units.end() - 1; itr != units.begin() - 1; --itr) {
        auto *unit = *itr;
        AlLogI(TAG, "%s will be destroy.", unit->toString().c_str());
        bool ret = unit->dispatch(msg);
    }
    clear();
    return true;
}

void UnitPipeline::clear() {
    AlLogI(TAG, "clear");
    for (auto *unit:units) {
        if (!unit->setting.hosted) {
            delete unit;
        }
    }
    units.clear();
    for (auto *unit:units0) {
        if (!unit->setting.hosted) {
            delete unit;
        }
    }
    units0.clear();
}

int UnitPipeline::registerAnUnit(Unit *unit) {
    {
//        std::lock_guard<std::mutex> guard(mtx);
        units0.push_back(unit);
    }
    _notifyCreate();
    return 1;
}

void UnitPipeline::_notifyCreate() {
    postEvent(AlMessage::obtain(EVENT_COMMON_PREPARE, nullptr, AlMessage::QUEUE_MODE_FIRST_ALWAYS));
}

void UnitPipeline::_notifyDestroy() {
    auto *msg = AlMessage::obtain(EVENT_COMMON_RELEASE);
    msg->desc = "Release";
    mHandler->sendMessage(msg);
}

void UnitPipeline::_notifyUnitsDesc() {
    auto clips = std::make_shared<AlVector<std::shared_ptr<FkUnitDesc>>>();

    for (auto *unit:units) {
        auto desc = std::make_shared<FkUnitDesc>();
        desc->name = unit->alias;
        for (auto it : unit->eventMap) {
            desc->msgVec.emplace_back(it.first);
        }
        clips->push_back(desc);
    }
    auto msg = AlMessage::obtain(MSG_ENGINE_NOTIFY_UNITS_DESC);
    msg->sp = clips;
    postEvent(msg);
}
