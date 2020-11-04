/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkUMonitor.h"
#include "TimeUtils.h"

FkUMonitor::FkUMonitor(string alias, AlUnitSetting setting) : Unit(alias, setting) {
    al_reg_msg(MSG_ENGINE_NOTIFY_UNITS_DESC, FkUMonitor::_onUnitsChanged);
    msgState = std::make_shared<FkMsgState>();
}

FkUMonitor::~FkUMonitor() {
}

bool FkUMonitor::onCreate(AlMessage *msg) {
    _notifyInfo(1);
    return true;
}

bool FkUMonitor::onDestroy(AlMessage *msg) {
    _notifyInfo(-1);
    return true;
}

bool FkUMonitor::onMsgReceived(AlMessage *msg) {
    msg->state.handleDoneInUS = TimeUtils::getCurrentTimeUS();
    msg->state.costTimeInUS = msg->state.handleDoneInUS - msg->state.handleTimeInUS;
    msg->state.copyTo(msgState);
    msgState->what = msg->what;
    if (msg->state.handleDoneInUS - mLastTime > 33000) {
        mLastTime = msg->state.handleDoneInUS;
        _notifyInfo(0);
    }
    return true;
}

bool FkUMonitor::_onUnitsChanged(AlMessage *msg) {
    mUnitsDesc = std::static_pointer_cast<AlVector<std::shared_ptr<FkUnitDesc>>>(msg->sp);
    return true;
}

void FkUMonitor::_notifyInfo(int monitorState) {
    if (onMonitorListener) {
        onMonitorListener(monitorState, mUnitsDesc, msgState);
    }
}

void FkUMonitor::setOnMonitorListener(OnMonitorListener l) {
    onMonitorListener = l;
}