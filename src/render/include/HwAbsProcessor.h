/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_HWABSPROCESSOR_H
#define HWVC_ANDROID_HWABSPROCESSOR_H

#include "Object.h"
#include <string>
#include "UnitPipeline.h"

using namespace std;

class HwAbsProcessor : public Object {
public:
    HwAbsProcessor(string name);

    virtual ~HwAbsProcessor();

    void post(function<void()> runnable);

protected:
    void startPipeline();

    void stopPipeline();

    void registerAnUnit(Unit *unit);

    void postEvent(Message *msg);

    void postEventAtFront(Message *msg);

    void removeAllMessage(int what);

private:
    string name;
    UnitPipeline *pipeline = nullptr;
};


#endif //HWVC_ANDROID_HWABSPROCESSOR_H
