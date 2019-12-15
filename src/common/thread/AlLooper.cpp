/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "AlLooper.h"
#include "AlHandler.h"
#include "Thread.h"
#include "Logcat.h"
#include "TimeUtils.h"
#include <cassert>

void AlLooper::prepare() {
    AlLooper *looper = AlLooperManager::getInstance()->create(Thread::currentThreadId());
    if (nullptr == looper) {
        Logcat::e("AlLooper", "AlLooper::prepare has been called.");
    }
    assert(nullptr != looper);
}

AlLooper *AlLooper::myLooper() {
    AlLooper *looper = AlLooperManager::getInstance()->get(Thread::currentThreadId());
    if (nullptr == looper) {
        Logcat::e("AlLooper", "Call AlLooper::prepare first pls.");
    }
    assert(nullptr != looper);
    return looper;
}

void AlLooper::loop() {
    myLooper()->_loop();
}

void AlLooper::exit() {
    AlLooperManager::getInstance()->remove(Thread::currentThreadId());
}

AlLooper::AlLooper() : Object() {
    looping = false;
    exiting = false;
    exited = false;
    exitSafely = false;
}

AlLooper::~AlLooper() {
    quit(false);
}

void AlLooper::sendMessage(AlMessage *msg) {
    if (exiting || exited)
        return;
    _enqueueMessage(msg);
}

void AlLooper::_enqueueMessage(AlMessage *msg) {
    switch (msg->queueMode) {
        case AlMessage::QUEUE_MODE_UNIQUE: {
            queue.removeAllMessage(msg->what);
            queue.offer(msg);
            break;
        }
        case AlMessage::QUEUE_MODE_FIRST_ALWAYS: {
            queue.offerAtFront(msg);
            break;
        }
        case AlMessage::QUEUE_MODE_CLEAR: {
            queue.removeAllMessage(msg->what);
            delete msg;
            break;
        }
        default: {
            queue.offer(msg);
            break;
        }
    }
}

void AlLooper::quit(bool safely) {
    if (exiting || exited)
        return;
    exitSafely = safely;
    exiting = true;
    queue.notify();
}

void AlLooper::_loop() {
    if (looping || exiting || exited) return;
    looping = true;
    for (;;) {
        queue.dump();
        AlMessage *msg = queue.take();
        if (msg) {
            if (msg->target) {
                msg->target->dispatchMessage(msg);
            }
            delete msg;
        }
        queue.pop();
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
        AlMessage *msg = queue.take();
        if (msg) {
            delete msg;
        }
        queue.pop();
    }
    queue.clear();
    Logcat::i("AlLooper", "%s(%d) clear message cost %lld us.", __FUNCTION__, __LINE__,
              TimeUtils::getCurrentTimeUS() - time);
    exiting = false;
    exited = true;
    looping = false;
}

AlLooperManager *AlLooperManager::instance = new AlLooperManager();

AlLooperManager *AlLooperManager::getInstance() {
    return instance;
}

AlLooper *AlLooperManager::create(long tid) {
    std::lock_guard<std::mutex> guard(mtx);
    auto it = looperMap.find(tid);
    if (looperMap.end() == it) {
        auto *looper = new AlLooper();
        looperMap[tid] = looper;
        return looper;
    }
    return nullptr;
}

AlLooper *AlLooperManager::get(long tid) {
    std::lock_guard<std::mutex> guard(mtx);
    auto it = looperMap.find(tid);
    if (looperMap.end() == it) {
        return nullptr;
    }
    return it->second;
}

void AlLooperManager::remove(long tid) {
    std::lock_guard<std::mutex> guard(mtx);
    auto it = looperMap.find(tid);
    if (looperMap.end() != it) {
        looperMap.erase(it);
        auto *looper = it->second;
        delete looper;
    }
}