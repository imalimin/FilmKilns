/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "../include/HandlerThread.h"
#include "../include/Thread.h"
#include "../include/log.h"

void HandlerThread::run() {
    while (true) {
//        this->printQueue();
        Message *msg = this->take();
        if (msg) {
            if (msg->runnable) {
                msg->runnable(msg);
            }
            delete msg;
        }
        if (this->requestQuit) {
            /** Quit safely. */
            if (this->requestQuitSafely) {
                if (this->size() <= 0) {
                    Logcat::i("HWVC", "HandlerThread(%s) requestQuitSafely", this->name.c_str());
                    break;
                }
            } else {/** Quit now. */
                break;
            }
        }
    }
    /** Quit! */
    Logcat::i("HWVC", "HandlerThread(%s) quit, left message count %d",
              this->name.c_str(),
              queue->size());
    this->printQueue();
}

HandlerThread::HandlerThread(string name) {
    this->name = name;
    queue = new MessageQueue();
    mThread = new thread(&HandlerThread::run, this);
}

HandlerThread::~HandlerThread() {
    quitSafely();
    if (this->queue) {
        delete this->queue;
        this->queue = nullptr;
    }
}

void HandlerThread::sendMessage(Message *msg) {
    if (!msg) {
        return;
    }
    simpleLock.lock();
    if (requestQuitSafely || requestQuit) {
        Logcat::e("HWVC", "HandlerThread had quited, skip message %p", msg);
        simpleLock.unlock();
        return;
    }
    simpleLock.unlock();
    queue->offer(msg);
}

void HandlerThread::sendMessageAtFront(Message *msg) {
    if (!msg) {
        return;
    }
    simpleLock.lock();
    if (requestQuitSafely || requestQuit) {
        Logcat::e("HWVC", "HandlerThread had quited, skip message %p", msg);
        simpleLock.unlock();
        return;
    }
    simpleLock.unlock();
    queue->offerAtFront(msg);
}

Message *HandlerThread::take() {
    return queue->take();
}

int HandlerThread::size() {
    return queue->size();
}

void HandlerThread::pop() {
    queue->pop();
}

void HandlerThread::quit() {
    simpleLock.lock();
    this->requestQuit = true;
    simpleLock.unlock();
    if (nullptr != queue) {
        queue->notify();
    }
    if (nullptr != mThread) {
        mThread->join();
        delete mThread;
        mThread = nullptr;
    }
}

void HandlerThread::quitSafely() {
    simpleLock.lock();
    this->requestQuitSafely = true;
    simpleLock.unlock();
    quit();
}

void HandlerThread::removeAllMessage(int what) {
    queue->removeAllMessage(what);
}

void HandlerThread::printQueue() {
    queue->printQueue();
}