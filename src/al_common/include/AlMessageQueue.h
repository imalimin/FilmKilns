/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_ALMESSAGEQUEUE_H
#define HWVC_ANDROID_ALMESSAGEQUEUE_H

#include "AlMessage.h"
#include <string>
#include <pthread.h>
#include <list>
#include "log.h"
#include "functional"

AL_CLASS AlMessageQueue AL_EXTEND Object {
public:
    AlMessageQueue();

    virtual ~AlMessageQueue();

    void offer(AlMessage *msg);

    void offerAtFront(AlMessage *msg);

    AlMessage *take();

    size_t size();

    bool empty();

    void notify();

    void clear();

    void removeAllMessage(const int what);

    void dump();

private:
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    std::list<AlMessage *> queue;
    bool invalid = false;
};


#endif //HWVC_ANDROID_ALMESSAGEQUEUE_H
