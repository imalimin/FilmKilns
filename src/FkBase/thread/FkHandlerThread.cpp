/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkHandlerThread.h"
#include <sys/prctl.h>

#define TAG "FkHandlerThread"

std::shared_ptr<FkHandlerThread> FkHandlerThread::create(std::string name) {
    return std::make_shared<FkHandlerThread>(name);
}

FkHandlerThread::FkHandlerThread(std::string name)
        : FkObject(),
          name(std::move(name)),
          mThread(thread(&FkHandlerThread::run, std::ref(*this))),
          exiting(false),
          exited(false) {
}

void FkHandlerThread::run() {
    pthread_setname_np(mThread.native_handle(), name.c_str());
    exited = false;
    exiting = false;
    FkLooper::prepare();
    mtx_l.lock();
    mLooper = FkLooper::myLooper();
    // 通知getLooper方法，Looper准备好了
    cond.notify_all();
    mtx_l.unlock();
    FkLogI(TAG, "Entry thread(%s) looper.", name.c_str());
    FkLooper::loop();
    FkLooper::exit();
    mtx_l.lock();
    exiting = false;
    mLooper = nullptr;
    mtx_l.unlock();
    exited = true;
    FkLogI(TAG, "Exist thread(%s).", name.c_str());
}

FkHandlerThread::~FkHandlerThread() {
    quit();
    mThread.join();
}

void FkHandlerThread::quit() {
    if (exiting || exited) {
        return;
    }
    exiting = true;
    FkLooper *looper = getLooper();
    if (looper) {
        looper->quit(false);
    }
}

bool FkHandlerThread::quitSafely() {
    if (exiting || exited) {
        return false;
    }
    exiting = true;
    FkLooper *looper = getLooper();
    if (looper) {
        looper->quit(true);
        return true;
    }
    return false;
}

FkLooper *FkHandlerThread::getLooper() {
    if (exited) {
        FkLogI(TAG, "Thread(%s) had exit.", name.c_str());
        return nullptr;
    }
    //TODO 这里mLooper可能为空
    std::unique_lock<std::mutex> lck(mtx_l);
    if (nullptr == mLooper) {
        FkLogI(TAG, "Thread(%s) waiting looper.", name.c_str());
        cond.wait(lck);
    }
    return mLooper;
}