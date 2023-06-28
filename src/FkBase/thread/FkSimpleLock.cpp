/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkSimpleLock.h"

FK_IMPL_CLASS_TYPE(FkSimpleLock, FkObject)

FkSimpleLock::FkSimpleLock() : FkObject() {
}

FkSimpleLock::~FkSimpleLock() {
    notify();
}

void FkSimpleLock::lock() {
    mtx.lock();
}

void FkSimpleLock::unlock() {
    mtx.unlock();
}

void FkSimpleLock::notify() {
    std::unique_lock<std::mutex> lck(mtx);
    cond.notify_all();
//    sem_post(&sem);//0变成1
}

void FkSimpleLock::wait() {
//    sem_wait(&sem);//1变成0，等待
    std::unique_lock<std::mutex> lck(mtx);
    cond.wait(lck);
}

void FkSimpleLock::wait(int us) {
    if (us <= 0) return;
    std::unique_lock<std::mutex> lck(mtx);
    cond.wait_for(lck, std::chrono::nanoseconds(us * 1000));
}

bool FkSimpleLock::wait(int us, std::function<bool()> func) {
    if (us <= 0) return false;
    std::unique_lock<std::mutex> lck(mtx);
    return cond.wait_for(lck, std::chrono::nanoseconds(us * 1000), func);
}