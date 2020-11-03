//
// Created by mingyi.li on 2020-11-03.
//

#ifndef HWVC_ANDROID_FKUMONITOR_H
#define HWVC_ANDROID_FKUMONITOR_H

#include "Unit.h"

AL_CLASS FkUMonitor AL_EXTEND Unit {
public:
    FkUMonitor(string alias, AlUnitSetting setting);

    virtual ~FkUMonitor();

private:
    bool onCreate(AlMessage *msg) override;

    bool onDestroy(AlMessage *msg) override;

    bool onMsgReceived(AlMessage *msg) override;

};


#endif //HWVC_ANDROID_FKUMONITOR_H
