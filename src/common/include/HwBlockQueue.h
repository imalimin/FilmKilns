//
// Created by mingyi.li on 2019/6/18.
//

#ifndef HWVC_ANDROID_HWBLOCKQUEUE_H
#define HWVC_ANDROID_HWBLOCKQUEUE_H

#include "Object.h"
#include <string>
#include <pthread.h>
#include <list>
#include "log.h"
#include "functional"

using namespace std;

template<class T>
class HwBlockQueue : public Object {
public:
    typedef list<T> Queue;

    HwBlockQueue() {
        pthread_mutex_init(&mutex, nullptr);
        pthread_cond_init(&cond, nullptr);
    }

    virtual ~HwBlockQueue() {
        LOGI("~HwBlockQueue");
        clear();
        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&cond);
    }

    /**
     * 阻塞式向队列增加一个元素
     */
    bool offer(const T &entity) {
        pthread_mutex_lock(&mutex);
//        if (size() >= SIZE_CACHE) {
//            pthread_cond_broadcast(cond);
//            pthread_mutex_unlock(mutex);
//            return false;
//        }

        m_queue.push_back(entity);

        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);
        return true;
    }

    bool offerAtFront(const T &entity) {
        pthread_mutex_lock(&mutex);

        m_queue.push_front(entity);

        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);
        return true;
    }

    /**
     * 阻塞式从队列拿出一个元素
     */
    T *take() {
        pthread_mutex_lock(&mutex);
        if (size() <= 0) {
            if (0 != pthread_cond_wait(&cond, &mutex)) {
                pthread_mutex_unlock(&mutex);
                return nullptr;
            }
        }
        T e = m_queue.front();
        m_queue.pop_front();

        pthread_mutex_unlock(&mutex);
        return &e;
    }

    /**
     * 删除所有元素
     */
    void clear() {
        notify();
        pthread_mutex_lock(&mutex);
        while (m_queue.size() > 0) {
            T e = m_queue.front();
            delete &e;
        }
        m_queue.clear();
        pthread_mutex_unlock(&mutex);
    }

    /**
     * 获取队列大小
     */
    int size() {
        return m_queue.size();
    }

    /**
     * 检查队列是否为空
     */
    bool isEmpty() {
        return m_queue.empty();
    }

    virtual void notify() override {
        Object::notify();
        pthread_cond_broadcast(&cond);
    }

    void remove(function<bool(T *e)> filter) {
        pthread_mutex_lock(&mutex);
//        m_queue.remove(filter);
        pthread_mutex_unlock(&mutex);
    }

private:
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    Queue m_queue;
};

#endif //HWVC_ANDROID_HWBLOCKQUEUE_H
