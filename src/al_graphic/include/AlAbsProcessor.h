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
#include "AlRunnable.h"

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

al_class AlAbsProcessor al_extend Unit {
public:
    AlAbsProcessor(string name);

    virtual ~AlAbsProcessor();

    void release();

    void release(AlRunnable *runnable);

    void post(function<void()> runnable);

protected:
    virtual void onCreate() ;

    virtual void onDestroy() ;

    virtual bool onCreate(AlMessage *msg) override ;

    virtual bool onDestroy(AlMessage *msg) override ;

    void registerAnUnit(Unit *unit);

private:
    string name;
    AlPostMan *pipeline = nullptr;
    AlRunnable *destroyRun = nullptr;
};


#endif //HWVC_ANDROID_ALABSPROCESSOR_H
