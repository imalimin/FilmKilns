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
//    typedef void (HwAbsProcessor::*Callback)(string, V);

    HwPairBuilder(HwPair<string, V> hwPair, function<void(string, HwPair<string, V> *)> callback)
            : Object(),
              hwPair(hwPair),
              callback(callback) {
    }

//    HwPairBuilder(const HwPairBuilder &builder) : Object(),
//                                                  hwPair(builder.hwPair),
//                                                  callback(builder.callback) {
//    }

    virtual ~HwPairBuilder() {
        callback = nullptr;
    }

    void to(initializer_list<string> args) {
        for (auto it = args.begin(); it != args.end(); ++it) {
            if (callback) {
                callback(*it, &hwPair);
            }
        }
    }

private:
    HwPair<string, V> hwPair;
    function<void(string, HwPair<string, V> *)> callback;
};

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

    HwPairBuilder<int32_t> *putInt32(string key, int32_t value);

    void putInt64(string unitAlias, string key, int64_t value);

    void putString(string unitAlias, string key, string value);

    void putObject(string unitAlias, string key, Object *value);

private:
    const string ALIAS_OF_MODEL_PROVIDER = "ModelProvider";
    string name;
    UnitPipeline *pipeline = nullptr;
    HwModelProvider *provider = nullptr;
};


#endif //HWVC_ANDROID_HWABSPROCESSOR_H
