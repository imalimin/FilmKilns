//
// Created by mingyi.li on 2018/12/25.
//

#ifndef HARDWAREVIDEOCODEC_MAINPIPELINE_H
#define HARDWAREVIDEOCODEC_MAINPIPELINE_H

#include "Object.h"
#include "AlHandlerThread.h"
#include "AlHandler.h"
#include "SimpleLock.h"
#include <vector>

using namespace std;

class Unit;

class UnitPipeline : public Object {
public:
    UnitPipeline(string name);

    virtual ~UnitPipeline();

    void postEvent(AlMessage *msg);

    virtual int registerAnUnit(Unit *unit);

    void postDestroy();

private:
    UnitPipeline(const UnitPipeline &o) : Object() {};

    void _dispatch(AlMessage *msg);

    bool _disCreate(AlMessage *msg);

    bool _disDestroy(AlMessage *msg);

    void clear();

    void _notifyCreate();

    void _notifyDestroy();

private:
    AlHandlerThread *mThread = nullptr;
    AlHandler *mHandler = nullptr;
    std::mutex mtx;
    /// Should prepare units.
    std::deque<Unit *> units0;
    vector<Unit *> units;
    bool notified = false;
};


#endif //HARDWAREVIDEOCODEC_MAINPIPELINE_H
