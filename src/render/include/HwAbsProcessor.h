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
#include "HwModelProvider.h"

using namespace std;

class HwAbsProcessor : public Object {
public:
    HwAbsProcessor(string name);

    virtual ~HwAbsProcessor();

    void post(function<void()> runnable);

protected:
    /**
     * Be call after release pipeline.
     */
    virtual void onDestroy();

    void registerAnUnit(Unit *unit);

    void postEvent(Message *msg);

    void putInt32(string unit, string key, int32_t value);

    void putInt64(string unit, string key, int64_t value);

    void putString(string unit, string key, string value);

    void putObject(string unit, string key, Object *value);

private:
    string name;
    UnitPipeline *pipeline = nullptr;
    HwModelProvider *provider = nullptr;
};


#endif //HWVC_ANDROID_HWABSPROCESSOR_H
