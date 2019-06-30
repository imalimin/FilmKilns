/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HARDWAREVIDEOCODEC_MESSAGEQUEUE_H
#define HARDWAREVIDEOCODEC_MESSAGEQUEUE_H

#include "Message.h"
#include <string>
#include <pthread.h>
#include <list>
#include "log.h"
#include "functional"

using namespace std;

class MessageQueue : public Object {
public:
    MessageQueue();

    virtual ~MessageQueue();

    void offer(Message *msg);

    void offerAtFront(Message *msg);

    Message *take();

    int size();

    void pop();

    virtual void notify() override;

    void clear();

    void removeAllMessage(const int what);

    void printQueue();

private:
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    list<Message *> queue;
};

#endif //HARDWAREVIDEOCODEC_MESSAGEQUEUE_H
