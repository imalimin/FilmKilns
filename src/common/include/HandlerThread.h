/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include <thread>
#include <pthread.h>
#include "MessageQueue.h"
#include "SimpleLock.h"

#ifndef HARDWAREVIDEOCODEC_HANDLERTHREAD_H
#define HARDWAREVIDEOCODEC_HANDLERTHREAD_H

class HandlerThread : public Object {
public:
    HandlerThread(string name);

    virtual ~HandlerThread();

    void sendMessage(Message *msg);

    void sendMessageAtFront(Message *msg);

    void removeAllMessage(int what);

    void quit();

    void quitSafely();

private:
    Message *take();

    void pop();

    int size();

    void run();

    void printQueue();

private:
    string name;
    std::thread *mThread = nullptr;
    MessageQueue *queue = nullptr;
    SimpleLock simpleLock;
    bool requestQuitSafely = false;
    bool requestQuit = false;
};


#endif //HARDWAREVIDEOCODEC_HANDLERTHREAD_H
