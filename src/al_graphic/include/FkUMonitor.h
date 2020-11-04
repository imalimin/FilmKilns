/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_FKUMONITOR_H
#define HWVC_ANDROID_FKUMONITOR_H

#include "Unit.h"
#include "AlVector.h"
#include "FkUnitDesc.h"

AL_CLASS FkUMonitor AL_EXTEND Unit {
public:
    typedef function<void(int, std::shared_ptr<AlVector<std::shared_ptr<FkUnitDesc>>>, std::shared_ptr<FkMsgState>)> OnMonitorListener;

    FkUMonitor(string alias, AlUnitSetting setting);

    virtual ~FkUMonitor();

    void setOnMonitorListener(OnMonitorListener l);

private:
    bool onCreate(AlMessage *msg) override;

    bool onDestroy(AlMessage *msg) override;

    bool onMsgReceived(AlMessage *msg) override;

    bool _onUnitsChanged(AlMessage *msg);

    void _notifyInfo(int monitorState);

private:
    std::shared_ptr<AlVector<std::shared_ptr<FkUnitDesc>>> mUnitsDesc;
    std::shared_ptr<FkMsgState> msgState;
    OnMonitorListener onMonitorListener = nullptr;
    int64_t mLastTime = 0;
};


#endif //HWVC_ANDROID_FKUMONITOR_H
