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
#include <initializer_list>
#include "UnitPipeline.h"
#include "Unit.h"
#include "HwPair.h"

using namespace std;

template<typename V>
class HwPairBuilder : public Object {
public:
    HwPairBuilder(UnitPipeline *pipe, int32_t what, HwPair<string, V> hwPair)
            : Object(),
              pipe(pipe),
              what(what),
              hwPair(hwPair) {
    }

    HwPairBuilder(const HwPairBuilder &builder) : Object(),
                                                  pipe(builder.pipe),
                                                  what(builder.what),
                                                  hwPair(builder.hwPair) {
    }

    virtual ~HwPairBuilder() {
        pipe = nullptr;
    }

    void to(initializer_list<string> args) {
        for (auto it = args.begin(); it != args.end(); ++it) {
            if (pipe) {
                AlMessage *msg = AlMessage::obtain(what, nullptr,
                                                   AlMessage::QUEUE_MODE_FIRST_ALWAYS);
                msg->obj = new HwPair<string, V>(
                        (*it) + "_" + hwPair.key(), hwPair.value());
                pipe->postEvent(msg);
            }
        }
    }

private:
    UnitPipeline *pipe = nullptr;
    int32_t what = 0;
    HwPair<string, V> hwPair;
};

class HwAbsProcessor : public Object {
public:
    HwAbsProcessor(string name);

    virtual ~HwAbsProcessor();

    void prepare();

    void post(function<void()> runnable);

protected:
    /**
     * Be call after release pipeline.
     */
    virtual void onDestroy();

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


#endif //HWVC_ANDROID_HWABSPROCESSOR_H
