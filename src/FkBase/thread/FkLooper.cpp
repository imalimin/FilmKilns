/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkLooper.h"
#include "FkHandler.h"
#include "Thread.h"
#include "FkDefinition.h"
#include "TimeUtils.h"

#define TAG "FkLooper"

void FkLooper::prepare() {
    FkLooper *looper = FkLooperManager::getInstance()->create(Thread::currentThreadId());
    if (nullptr == looper) {
        FkLogE(TAG, "has been called.");
    }
    FkAssert(nullptr != looper, );
}

FkLooper *FkLooper::myLooper() {
    FkLooper *looper = FkLooperManager::getInstance()->get(Thread::currentThreadId());
    if (nullptr == looper) {
        FkLogE(TAG, "Call first pls.");
    }
    FkAssert(nullptr != looper, nullptr);
    return looper;
}

int64_t FkLooper::myLooperId() {
    return reinterpret_cast<int64_t>(myLooper());
}

void FkLooper::loop() {
    myLooper()->_loop();
}

void FkLooper::exit() {
    FkLooperManager::getInstance()->remove(Thread::currentThreadId());
}

FkLooper::FkLooper() : FkObject() {
    looping = false;
    exiting = false;
    exited = false;
    exitSafely = false;
}

FkLooper::~FkLooper() {
    quit(false);
}

void FkLooper::sendMessage(std::shared_ptr<FkMessage> msg) {
    if (exiting || exited)
        return;
    _enqueueMessage(msg);
}

void FkLooper::_enqueueMessage(std::shared_ptr<FkMessage> msg) {
//    if (msg->queueMode & AlMessage::QUEUE_MODE_CLEAR_ALL) {
//        queue.clear();
//    }
    if (msg->flags & FkMessage::FLAG_UNIQUE) {
        queue.removeAllMessage(msg->what);
        queueLevel0.removeAllMessage(msg->what);
    }
    if (msg->flags & FkMessage::FLAG_CLEAR) {
        queue.removeAllMessage(msg->what);
        queueLevel0.removeAllMessage(msg->what);
    } else {
        if (msg->flags & FkMessage::FLAG_FIRST_ALWAYS) {
            queueLevel0.offer(msg);
        } else {
            queue.offer(msg);
        }
    }
}

void FkLooper::quit(bool safely) {
    if (exiting || exited)
        return;
    exitSafely = safely;
    exiting = true;
    queue.notify();
}

void FkLooper::_loop() {
    if (looping || exiting || exited) return;
    looping = true;
    for (;;) {
        queue.dump();
        auto msg = _take();
        if (msg) {
            if (msg->target) {
                msg->target->dispatchMessage(msg);
            }
            FkMessage::recycle(msg);
        }
        if (exitSafely) {
            if (exiting && 0 == queue.size()) {
                break;
            }
        } else {
            if (exiting) {
                break;
            }
        }
    }
    int64_t time = TimeUtils::getCurrentTimeUS();
    while (0 != queue.size()) {
        auto msg = queue.take();
    }
    queue.clear();
    FkLogI(TAG, "clear message cost %lld us.", TimeUtils::getCurrentTimeUS() - time);
    exiting = false;
    exited = true;
    looping = false;
}

std::shared_ptr<FkMessage> FkLooper::_take() {
    if (queueLevel0.empty()) {
        return queue.take();
    } else {
        return queueLevel0.take();
    }
}

FkLooperManager *FkLooperManager::instance = new FkLooperManager();

FkLooperManager *FkLooperManager::getInstance() {
    return instance;
}

FkLooper *FkLooperManager::create(long tid) {
    std::lock_guard<std::mutex> guard(mtx);
    auto it = looperMap.find(tid);
    if (looperMap.end() == it) {
        auto *looper = new FkLooper();
        looperMap[tid] = looper;
        return looper;
    }
    return nullptr;
}

FkLooper *FkLooperManager::get(long tid) {
    std::lock_guard<std::mutex> guard(mtx);
    auto it = looperMap.find(tid);
    if (looperMap.end() == it) {
        return nullptr;
    }
    return it->second;
}

void FkLooperManager::remove(long tid) {
    std::lock_guard<std::mutex> guard(mtx);
    auto it = looperMap.find(tid);
    if (looperMap.end() != it) {
        looperMap.erase(it);
        auto *looper = it->second;
        delete looper;
    }
}