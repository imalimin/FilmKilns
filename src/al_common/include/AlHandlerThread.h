/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_ALHANDLERTHREAD_H
#define HWVC_ANDROID_ALHANDLERTHREAD_H

#include "Object.h"
#include "AlHandler.h"
#include "AlLooper.h"
#include <thread>
#include <mutex>
#include <condition_variable>

al_class AlHandlerThread al_extend Object {
public:
    static AlHandlerThread *create(std::string name);

public:
    ~AlHandlerThread();

    void quit();

    bool quitSafely();

    AlLooper *getLooper();

private:
    AlHandlerThread(std::string name);

    AlHandlerThread(AlHandlerThread &e) {}

    void run();

private:
    std::string name;
    std::thread mThread;
    AlLooper *mLooper = nullptr;
    std::mutex mtx_l;
    std::condition_variable cond;
    std::atomic_bool exiting;
    std::atomic_bool exited;
};


#endif //HWVC_ANDROID_ALHANDLERTHREAD_H
