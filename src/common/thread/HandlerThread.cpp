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
        pthread_mutex_lock(&mutex);
        if (shouldQuit()) {
            break;
        }
        pthread_mutex_unlock(&mutex);
//        this->printQueue();
        Message *msg = this->take();
        if (this->requestQuit && !this->requestQuitSafely) {
            if (msg) {
                Logcat::i("HWVC", "HandlerThread(%s) requestQuit, %d, %ld",
                          this->name.c_str(),
                          msg->what,
                          Thread::currentThreadId());
            } else {
                Logcat::i("HWVC", "HandlerThread(%s)  requestQuit", this->name.c_str());
            }
            break;
        }
        if (!msg) {
            continue;
        }
        msg->runnable(msg);
        delete msg;
        if (this->requestQuitSafely && this->size() <= 0) {
            Logcat::i("HWVC", "HandlerThread(%s) requestQuitSafely", this->name.c_str());
            break;
        }
    }
    Logcat::i("HWVC", "HandlerThread(%s) quit, left message count %d",
              this->name.c_str(),
              queue->size());
    this->printQueue();
}

HandlerThread::HandlerThread(string name) {
    this->name = name;
    pthread_mutex_init(&mutex, nullptr);
    queue = new MessageQueue();
    mThread = new thread(&HandlerThread::run, this);
}

bool HandlerThread::shouldQuit() {
    if (this->requestQuit && !this->requestQuitSafely) {
        return true;
    }
    return this->requestQuitSafely && size() <= 0;
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
    pthread_mutex_lock(&mutex);
    this->requestQuit = true;
    pthread_mutex_unlock(&mutex);
    if (nullptr != queue) {
        queue->notify();
    }
    if (nullptr != mThread) {
        mThread->join();
        delete mThread;
        mThread = nullptr;
    }
    pthread_mutex_destroy(&mutex);
}

void HandlerThread::quitSafely() {
    if (0 != size()) {
        this->requestQuitSafely = true;
    }
    quit();
}

void HandlerThread::removeAllMessage(int what) {
    queue->removeAllMessage(what);
}

void HandlerThread::printQueue() {
    queue->printQueue();
}