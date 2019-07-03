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
            msg->runnable(msg);
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
    if (requestQuitSafely || requestQuit) {
        LOGE("HandlerThread had quited");
        Logcat::i("HWVC", "HandlerThread skip message %p", msg);
        return;
    }
    offer(msg);
}

void HandlerThread::sendMessageAtFront(Message *msg) {
    if (requestQuitSafely || requestQuit) {
        LOGE("HandlerThread had quited");
        return;
    }
    queue->offerAtFront(msg);
}

void HandlerThread::offer(Message *msg) {
    queue->offer(msg);
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
    this->requestQuit = true;
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
    this->requestQuitSafely = true;
    quit();
}

void HandlerThread::removeAllMessage(int what) {
    queue->removeAllMessage(what);
}

void HandlerThread::printQueue() {
    queue->printQueue();
}