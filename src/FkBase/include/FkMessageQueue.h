/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_BASE_FKMESSAGEQUEUE_H
#define FK_BASE_FKMESSAGEQUEUE_H

#include "FkObject.h"
#include "FkMessage.h"

FK_SUPER_CLASS(FkMessageQueue, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkMessageQueue)

public:
    FkMessageQueue() {
        pthread_mutex_init(&mutex, nullptr);
        pthread_cond_init(&cond, nullptr);
    }

    FkMessageQueue(const FkMessageQueue &o) = delete;

    virtual ~FkMessageQueue() {
        pthread_mutex_lock(&mutex);
        invalid = true;
        pthread_mutex_unlock(&mutex);
        clear();
        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&cond);
    }

    void offer(std::shared_ptr<FkMessage> msg) {
        pthread_mutex_lock(&mutex);
        if (invalid) {
            pthread_mutex_unlock(&mutex);
            return;
        }
        queue.push_back(msg);

        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);
    }

    void offerAtFront(std::shared_ptr<FkMessage> msg) {
        pthread_mutex_lock(&mutex);
        if (invalid) {
            pthread_mutex_unlock(&mutex);
            return;
        }

        queue.push_front(msg);
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);
    }

    std::shared_ptr<FkMessage> take() {
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
        auto e = queue.front();
        queue.pop_front();

        pthread_mutex_unlock(&mutex);
        return e;
    }

    size_t size() {
        return queue.size();
    }

    bool empty() {
        return 0 == size();
    }

    void notify() {
        pthread_mutex_lock(&mutex);
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);
    }

    void clear() {
        notify();
        if (queue.empty()) {
            return;
        }
        pthread_mutex_lock(&mutex);
        while (!queue.empty()) {
            auto e = queue.front();
            queue.pop_front();
        }
        queue.clear();
        pthread_mutex_unlock(&mutex);
    }

    void removeAllMessage(const int what) {
        pthread_mutex_lock(&mutex);
        if (invalid) {
            pthread_mutex_unlock(&mutex);
            return;
        }
        auto itr = queue.begin();
        while (itr != queue.end()) {
            auto e = *itr;
            if (what == e->what) {
                itr = queue.erase(itr);
                continue;
            }
            ++itr;
        }
        pthread_mutex_unlock(&mutex);
    }

    void dump() {
#if 0
        std::ostringstream os;
    auto itr = queue.begin();
    while (itr != queue.end()) {
        auto e = *itr;
        os << e->desc << "(" << e << "),";
        ++itr;
    }
    Logcat::i(TAG, "%s", os.str().c_str());
#endif
    }

private:
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    std::list<std::shared_ptr<FkMessage>> queue;
    bool invalid = false;

};


#endif //FK_BASE_FKMESSAGEQUEUE_H
