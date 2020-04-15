/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HARDWAREVIDEOCODEC_SIMPLELOCK_H
#define HARDWAREVIDEOCODEC_SIMPLELOCK_H

#include "Object.h"
#include <mutex>
#include <condition_variable>

class SimpleLock : public Object {
public:
    SimpleLock();

    virtual ~SimpleLock();

    void lock();

    void unlock();

    void wait();

    void wait(int us);

    void notify();

private:
    std::mutex mtx;
    std::condition_variable cond;
};


#endif //HARDWAREVIDEOCODEC_SIMPLELOCK_H
