/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_ALLOOPER_H
#define HWVC_ANDROID_ALLOOPER_H

#include "Object.h"
#include "AlMessage.h"
#include "AlMessageQueue.h"
#include <map>
#include <mutex>
#include <atomic>

class AlLooperManager;

class AlLooper : public Object {
public:
    /**
     * 为线程准备一个Looper，如果线程已经存在Looper，则报错
     */
    static void prepare();

    /**
     * 循环消费消息
     */
    static void loop();

    /**
     * 获取当前线程的Looper
     * @return 前线程的Looper
     */
    static AlLooper *myLooper();

    static void exit();

    void sendMessage(AlMessage *msg);

    void quit(bool safely);

    void dump();

private:
    friend AlLooperManager;

    AlLooper();

    AlLooper(AlLooper &e) : Object() {}

    ~AlLooper();

    void _loop();

    void _enqueueMessage(AlMessage *msg);

    AlMessage *_take();

    void _pop();

private:
    std::atomic_bool exiting, exited, exitSafely;
    std::atomic_bool looping;
    AlMessageQueue queue;
    AlMessageQueue queueLevel0;
};

class AlLooperManager : public Object {
private:
    friend AlLooper;

    static AlLooperManager *getInstance();

private:
    AlLooperManager() : Object() {}

    AlLooperManager(AlLooperManager &e) : Object() {}

    ~AlLooperManager() {}

    /**
     * 为当前线程创建Looper
     * @return 当前线程的Looper
     */
    AlLooper *create(long tid);

    /**
     * 获取当前线程的Looper
     * @return 当前线程的Looper
     */
    AlLooper *get(long tid);

    /**
     * 销毁当前线程的Looper
     */
    void remove(long tid);

private:
    static AlLooperManager *instance;
    std::map<long, AlLooper *> looperMap;
    std::mutex mtx;
};


#endif //HWVC_ANDROID_ALLOOPER_H
