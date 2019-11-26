/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "AlHandlerThread.h"
#include "AlLooper.h"
#include "Logcat.h"

#define TAG "AlHandlerThread"

AlHandlerThread *AlHandlerThread::create(std::string name) {
    return new AlHandlerThread(name);
}

AlHandlerThread::AlHandlerThread(std::string name)
        : Object(),
          name(name),
          exited(false),
          exiting(false) {
}

void AlHandlerThread::run() {
    exited = false;
    exiting = false;
    AlLooper::prepare();
    mtx_l.lock();
    mLooper = AlLooper::myLooper();
    // 通知getLooper方法，Looper准备好了
    cond.notify_all();
    mtx_l.unlock();
    Logcat::i(TAG, "%s(%d) loop.", __FUNCTION__, __LINE__);
    AlLooper::loop();
    AlLooper::exit();
    mtx_l.lock();
    exiting = false;
    mLooper = nullptr;
    mtx_l.unlock();
    exited = true;
    Logcat::i(TAG, "%s(%d) exit.", __FUNCTION__, __LINE__);
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
    if (exited) {
        Logcat::i(TAG, "%s(%d) thread had exit.", __FUNCTION__, __LINE__);
        return nullptr;
    }
    //TODO 这里mLooper可能为空
    std::unique_lock<std::mutex> lck(mtx_l);
    if (nullptr == mLooper) {
        Logcat::i(TAG, "%s(%d) wait.", __FUNCTION__, __LINE__);
        cond.wait(lck);
    }
    return mLooper;
}