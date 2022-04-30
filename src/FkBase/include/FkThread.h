/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: ${author}
* CREATE TIME: 2022-3-26 22:24:30
*/

#ifndef FK_BASE_FKTHREAD_H
#define FK_BASE_FKTHREAD_H

#include "FkObject.h"
#include <pthread.h>
#include <thread>
#include <string>
#include <functional>

FK_SUPER_CLASS(FkThread, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkThread)

public:
    static bool sleep(int64_t us);

    static long currentThreadId();

public:
    std::string name;
    std::function<void()> runnable;

    FkThread(std::string name, std::function<void()> runnable);

    void start();

    bool isRunning();

    void interrupt();

    bool interrupted();

    virtual ~FkThread();

    void stop();

private:
    pthread_attr_t attr;
    pthread_t thread;
    pthread_mutex_t mutex;
    bool inter = false;

    void createThread();

    void lock();

    void unLock();
};

#endif //FK_BASE_FKTHREAD_H