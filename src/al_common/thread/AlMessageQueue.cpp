/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "AlMessageQueue.h"
#include <sstream>

#define TAG "AlMessageQueue"

AlMessageQueue::AlMessageQueue() {
    pthread_mutex_init(&mutex, nullptr);
    pthread_cond_init(&cond, nullptr);
}

AlMessageQueue::~AlMessageQueue() {
    LOGI("MessageQueue::~MessageQueue");
    pthread_mutex_lock(&mutex);
    invalid = true;
    pthread_mutex_unlock(&mutex);
    clear();
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}

void AlMessageQueue::offer(AlMessage *msg) {
    pthread_mutex_lock(&mutex);
    if (invalid) {
        pthread_mutex_unlock(&mutex);
        return;
    }
    queue.push_back(msg);

    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
}

void AlMessageQueue::offerAtFront(AlMessage *msg) {
    pthread_mutex_lock(&mutex);
    if (invalid) {
        pthread_mutex_unlock(&mutex);
        return;
    }

    queue.push_front(msg);
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
}


AlMessage *AlMessageQueue::take() {
    pthread_mutex_lock(&mutex);
    if (invalid) {
        pthread_mutex_unlock(&mutex);
        return nullptr;
    }
    if (size() <= 0) {
        if (0 != pthread_cond_wait(&cond, &mutex)) {
            pthread_mutex_unlock(&mutex);
            return nullptr;
        }
    }
    if (queue.empty()) {
        pthread_mutex_unlock(&mutex);
        return nullptr;
    }
    /**
     * Here maybe crash if queue is empty.
     */
    AlMessage *e = queue.front();
    queue.pop_front();

    pthread_mutex_unlock(&mutex);
    return e;
}

size_t AlMessageQueue::size() {
    return queue.size();
}

void AlMessageQueue::notify() {
    pthread_mutex_lock(&mutex);
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
}

void AlMessageQueue::clear() {
    notify();
    if (queue.empty()) {
        return;
    }
    pthread_mutex_lock(&mutex);
    while (!queue.empty()) {
        auto *e = queue.front();
        queue.pop_front();
        if (e) {
            delete e;
        }
    }
    queue.clear();
    pthread_mutex_unlock(&mutex);
}

void AlMessageQueue::removeAllMessage(const int what) {
    pthread_mutex_lock(&mutex);
    if (invalid) {
        pthread_mutex_unlock(&mutex);
        return;
    }
    list<AlMessage *>::iterator itr = queue.begin();
    while (itr != queue.end()) {
        AlMessage *e = *itr;
        if (what == e->what) {
            delete e;
            itr = queue.erase(itr);
            continue;
        }
        ++itr;
    }
    pthread_mutex_unlock(&mutex);
}

void AlMessageQueue::dump() {
#if 0
    std::ostringstream os;
    list<AlMessage *>::iterator itr = queue.begin();
    while (itr != queue.end()) {
        AlMessage *e = *itr;
        os << e->desc << "(" << e << "),";
        ++itr;
    }
    Logcat::i(TAG, "%s", os.str().c_str());
#endif
}

bool AlMessageQueue::empty() {
    return 0 == size();
}
