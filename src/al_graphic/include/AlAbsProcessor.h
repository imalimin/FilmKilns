/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALABSPROCESSOR_H
#define HWVC_ANDROID_ALABSPROCESSOR_H

#include "Object.h"
#include "AlPostMan.h"
#include "Unit.h"
#include "HwPair.h"
#include <string>
#include <initializer_list>

template<typename V>
class HwPairBuilder : public Object {
public:
    HwPairBuilder(AlPostMan *pipe, int32_t what, HwPair<string, V> hwPair)
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
                AlMessage *msg = AlMessage::obtain(what, new HwPair<string, V>(
                        (*it) + "_" + hwPair.key(), hwPair.value()),
                                                   AlMessage::QUEUE_MODE_FIRST_ALWAYS);
                pipe->postEvent(msg);
            }
        }
    }

private:
    AlPostMan *pipe = nullptr;
    int32_t what = 0;
    HwPair<string, V> hwPair;
};

al_class_ex(AlAbsProcessor, Unit) {
public:
    AlAbsProcessor(string name);

    virtual ~AlAbsProcessor();

    void release();

    void post(function<void()> runnable);

protected:
    virtual void onCreate() ;

    virtual void onDestroy() ;

    virtual bool onCreate(AlMessage *msg) override ;

    virtual bool onDestroy(AlMessage *msg) override ;

    void registerAnUnit(Unit *unit);

    HwPairBuilder<int32_t> putInt32(string key, int32_t value);

    HwPairBuilder<int64_t> putInt64(string key, int64_t value);

    HwPairBuilder<string> putString(string key, string value);

    HwPairBuilder<Object *> putObject(string key, Object *value);

private:
    const string ALIAS_OF_MODEL_PROVIDER = "ModelProvider";
    string name;
    AlPostMan *pipeline = nullptr;
    HwModelProvider *provider = nullptr;

};


#endif //HWVC_ANDROID_ALABSPROCESSOR_H
