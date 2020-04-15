/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/SimpleLock.h"

SimpleLock::SimpleLock() : Object() {
}

SimpleLock::~SimpleLock() {
    notify();
}

void SimpleLock::lock() {
    mtx.lock();
}

void SimpleLock::unlock() {
    mtx.unlock();
}

void SimpleLock::notify() {
    std::unique_lock<std::mutex> lck(mtx);
    cond.notify_all();
//    sem_post(&sem);//0变成1
}

void SimpleLock::wait() {
//    sem_wait(&sem);//1变成0，等待
    std::unique_lock<std::mutex> lck(mtx);
    cond.wait(lck);
}

void SimpleLock::wait(int us) {
    if (us <= 0) return;
    std::unique_lock<std::mutex> lck(mtx);
    cond.wait_for(lck, chrono::nanoseconds(us * 1000));
}