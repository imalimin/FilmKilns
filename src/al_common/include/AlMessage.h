/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_ALMESSAGE_H
#define HWVC_ANDROID_ALMESSAGE_H

#include "Object.h"
#include <queue>
#include <mutex>

class AlLooper;

class AlHandler;

class AlMessageManager;

al_class AlMessage al_extend Object {
public:
    int32_t what = 0;
    int32_t arg1 = 0;
    int64_t arg2 = 0;
    string desc;
    /// Will be delete after cycle.
    Object *obj = nullptr;
    std::shared_ptr<Object> sp;
    int16_t queueMode = QUEUE_MODE_NORMAL;
private:
    friend AlLooper;
    friend AlHandler;
    AlHandler *target = nullptr;

private:
    AlMessage();

    AlMessage(int32_t what);

    AlMessage(int32_t what, Object *obj);

    AlMessage(int32_t what, Object *obj, int16_t queueMode);

    AlMessage(AlMessage &e) : Object() {
    }

public:
    virtual ~AlMessage();

    template<class T>
    T getObj() {
        if (nullptr != obj) {
            return dynamic_cast<T>(obj);
        }
        return nullptr;
    }

public:
    static constexpr int16_t QUEUE_MODE_NORMAL = 0x01;
    static constexpr int16_t QUEUE_MODE_UNIQUE = 0x02;
    static constexpr int16_t QUEUE_MODE_FIRST_ALWAYS = 0x04;
    static constexpr int16_t QUEUE_MODE_CLEAR = 0x08;
//    static constexpr int16_t QUEUE_MODE_CLEAR_ALL = 0x10;

public:
    static AlMessage *obtain(int32_t what);

    static AlMessage *obtain(int32_t what, Object *obj);

    static AlMessage *obtain(int32_t what, int16_t queueMode);

    static AlMessage *obtain(int32_t what, Object *obj, int16_t queueMode);
};

al_class AlMessageManager al_extend Object {
private:
    friend AlMessage;

    static AlMessageManager *getInstance();

public:

    void recycle(AlMessage *msg);

    AlMessage *popOne();

private:
    static AlMessageManager *instance;
    std::queue<AlMessage *> pool;
    std::mutex poolMtx;

    AlMessageManager() : Object() {}

    AlMessageManager(AlMessageManager &e) : Object() {}
};


#endif //HWVC_ANDROID_ALMESSAGE_H
