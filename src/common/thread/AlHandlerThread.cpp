/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "AlHandlerThread.h"
#include "AlLooper.h"
#include "Logcat.h"

AlHandlerThread *AlHandlerThread::create(std::string name) {
    return new AlHandlerThread(name);
}

AlHandlerThread::AlHandlerThread(std::string name) : Object(), name(name) {
}

void AlHandlerThread::run() {
    exited = false;
    exiting = false;
    AlLooper::prepare();
    mtx_l.lock();
    mLooper = AlLooper::myLooper();
    mtx_l.unlock();
    // 通知getLooper方法，Looper准备好了
    lock.notify();
    AlLooper::loop();
    AlLooper::exit();
    mtx_l.lock();
    exiting = false;
    exited = true;
    mLooper = nullptr;
    mtx_l.unlock();
    Logcat::i("AlHandlerThread", "%s(%d) exit.", __FUNCTION__, __LINE__);
}

AlHandlerThread::~AlHandlerThread() {
    quit();
    mThread.join();
}

void AlHandlerThread::quit() {
    if (exiting || exited) {
        return;
    }
    exiting = true;
    AlLooper *looper = getLooper();
    if (looper) {
        looper->quit(false);
    }
}

bool AlHandlerThread::quitSafely() {
    if (exiting || exited) {
        return false;
    }
    exiting = true;
    AlLooper *looper = getLooper();
    if (looper) {
        looper->quit(true);
        return true;
    }
    return false;
}

AlLooper *AlHandlerThread::getLooper() {
    //TODO 这里mLooper可能为空
    std::lock_guard<std::mutex> guard(mtx_h);
    if (!exited && nullptr == mLooper) {
        lock.wait();
    }
    if (exited) {
        Logcat::i("AlHandlerThread", "%s(%d) thread had exit.", __FUNCTION__, __LINE__);
    }
    return mLooper;
}