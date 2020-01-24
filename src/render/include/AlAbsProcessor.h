/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALABSPROCESSOR_H
#define HWVC_ANDROID_ALABSPROCESSOR_H

#include "Object.h"
#include "UnitPipeline.h"
#include "Unit.h"
#include "HwPair.h"
#include "HwAbsProcessor.h"
#include <string>
#include <initializer_list>

al_class_ex(AlAbsProcessor, Unit) {
public:
    AlAbsProcessor(string name);

    virtual ~AlAbsProcessor();

    void prepare();

    void release();

    void post(function<void()> runnable);

protected:
    virtual void onCreate() ;

    virtual void onDestroy() ;

    virtual bool onCreate(AlMessage *msg) override ;

    virtual bool onDestroy(AlMessage *msg) override ;

    void registerAnUnit(Unit *unit);

    void postEvent(AlMessage *msg);

    HwPairBuilder<int32_t> putInt32(string key, int32_t value);

    HwPairBuilder<int64_t> putInt64(string key, int64_t value);

    HwPairBuilder<string> putString(string key, string value);

    HwPairBuilder<Object *> putObject(string key, Object *value);

private:
    const string ALIAS_OF_MODEL_PROVIDER = "ModelProvider";
    string name;
    UnitPipeline *pipeline = nullptr;
    HwModelProvider *provider = nullptr;

};


#endif //HWVC_ANDROID_ALABSPROCESSOR_H
