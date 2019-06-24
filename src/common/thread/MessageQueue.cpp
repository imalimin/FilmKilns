/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "../include/MessageQueue.h"
#include <sstream>

MessageQueue::MessageQueue() {
    pthread_mutex_init(&mutex, nullptr);
    pthread_cond_init(&cond, nullptr);
}

MessageQueue::~MessageQueue() {
    LOGI("MessageQueue::~MessageQueue");
    clear();
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}

void MessageQueue::offer(Message *msg) {
    pthread_mutex_lock(&mutex);

    queue.push_back(msg);

    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
}

void MessageQueue::offerAtFront(Message *msg) {
    pthread_mutex_lock(&mutex);

    queue.push_front(msg);
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
}


Message *MessageQueue::take() {
    pthread_mutex_lock(&mutex);
    if (size() <= 0) {
        if (0 != pthread_cond_wait(&cond, &mutex)) {
            pthread_mutex_unlock(&mutex);
            return nullptr;
        }
    }
    Message *e = queue.front();
    queue.pop_front();

    pthread_mutex_unlock(&mutex);
    return e;
}

int MessageQueue::size() {
    return queue.size();
};

void MessageQueue::pop() {
//    queue->pop();
}

void MessageQueue::notify() {
    pthread_cond_broadcast(&cond);
}

void MessageQueue::clear() {
    notify();
    pthread_mutex_lock(&mutex);
    while (!queue.empty()) {
        Message *e = queue.front();
        if (e) {
            delete e;
        }
        queue.pop_front();
    }
    queue.clear();
    pthread_mutex_unlock(&mutex);
}

void MessageQueue::removeAllMessage(const int what) {
    pthread_mutex_lock(&mutex);
    list<Message *>::iterator itr = queue.begin();
    while (itr != queue.end()) {
        Message *e = *itr;
        if (what == e->what) {
            queue.remove(e);
            delete e;
        }
        ++itr;
    }
    pthread_mutex_unlock(&mutex);
}

void MessageQueue::printQueue() {
#if 1
    std::ostringstream os;
    list<Message *>::iterator itr = queue.begin();
    while (itr != queue.end()) {
        Message *e = *itr;
        os << e->msg << "(" << e << "),";
        ++itr;
    }
    Logcat::i("HWVC", "MessageQueue(%s)", os.str().c_str());
#endif
}