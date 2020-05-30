/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALSAFEQUEUE_H
#define HWVC_ANDROID_ALSAFEQUEUE_H

#include "AlQueue.h"

template<class T>
AL_CLASS AlSafeQueue AL_EXTEND AlQueue<T> {
public:
    AlSafeQueue() : AlQueue<T>() {

    }

    virtual ~AlSafeQueue() {

    }

    virtual void push(const T &o) override {
        std::lock_guard<std::mutex> guard(mtx);
        AlQueue<T>::push(o);
    }

    virtual T pop() override {
        std::lock_guard<std::mutex> guard(mtx);
        return AlQueue<T>::pop();
    }

    virtual size_t size() override {
        std::lock_guard<std::mutex> guard(mtx);
        return AlQueue<T>::size();
    }

    virtual bool empty() override {
        std::lock_guard<std::mutex> guard(mtx);
        return AlQueue<T>::empty();
    }

private:
    AlSafeQueue(const AlSafeQueue &o) : Object() {

    }

private:
    std::mutex mtx;
};

#endif //HWVC_ANDROID_ALSAFEQUEUE_H
