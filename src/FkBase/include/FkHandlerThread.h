/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_BASE_FKHANDLERTHREAD_H
#define FK_BASE_FKHANDLERTHREAD_H

#include "FkObject.h"
#include "FkHandler.h"
#include "FkLooper.h"
#include <thread>
#include <mutex>
#include <condition_variable>

FK_CLASS FkHandlerThread FK_EXTEND FkObject {
public:
    static std::shared_ptr<FkHandlerThread> create(std::string name);

public:
    FkHandlerThread(std::string name);

    FkHandlerThread(const FkHandlerThread &o) = delete;

    ~FkHandlerThread();

    void quit();

    bool quitSafely();

    FkLooper *getLooper();

private:
    void run();

private:
    std::string name;
    std::thread mThread;
    FkLooper *mLooper = nullptr;
    std::mutex mtx_l;
    std::condition_variable cond;
    std::atomic_bool exiting;
    std::atomic_bool exited;

};


#endif //FK_BASE_FKHANDLERTHREAD_H
