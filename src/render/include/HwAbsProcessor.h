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
#include "HwAbsPipelineModel.h"

using namespace std;

class HwAbsProcessor : public Object {
public:
    HwAbsProcessor(string name);

    virtual ~HwAbsProcessor();

    void post(function<void()> runnable);

    HwAbsPipelineModel *getModel();

protected:
    void startPipeline();

    void stopPipeline();

    void registerAnUnit(Unit *unit);

    void postEvent(Message *msg);

    void postEventAtFront(Message *msg);

    void removeAllMessage(int what);

    virtual HwAbsPipelineModel *createModel() = 0;

private:
    void _createModel();

private:
    string name;
    UnitPipeline *pipeline = nullptr;
    HwAbsPipelineModel *model = nullptr;
};


#endif //HWVC_ANDROID_HWABSPROCESSOR_H
