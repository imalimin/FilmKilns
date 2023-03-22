/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_BASE_FKSIMPLELOCK_H
#define FK_BASE_FKSIMPLELOCK_H

#include "FkObject.h"
#include <mutex>
#include <condition_variable>

FK_SUPER_CLASS(FkSimpleLock, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkSimpleLock)

public:
    FkSimpleLock();

    virtual ~FkSimpleLock();

    void lock();

    void unlock();

    void wait();

    void wait(int us);

    void notify();

private:
    std::mutex mtx;
    std::condition_variable cond;
};


#endif //FK_BASE_FKSIMPLELOCK_H
