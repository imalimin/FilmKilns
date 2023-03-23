/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_BASE_FKSAFEQUEUE_H
#define FK_BASE_FKSAFEQUEUE_H

#include "FkQueue.h"
#include <mutex>

FK_SUPER_TEMPLATE_CLASS(FkSafeQueue, FkQueue<T>, class T) {
FK_DEF_CLASS_TYPE_FUNC(FkSafeQueue)

public:
    FkSafeQueue() : FkQueue<T>() {}

    virtual ~FkSafeQueue() {}

    virtual void push(const T &o) override {
        std::lock_guard<std::mutex> guard(mtx);
        FkQueue<T>::push(o);
    }

    virtual T pop() override {
        std::lock_guard<std::mutex> guard(mtx);
        return FkQueue<T>::pop();
    }

    virtual size_t size() override {
        std::lock_guard<std::mutex> guard(mtx);
        return FkQueue<T>::size();
    }

    virtual bool empty() override {
        std::lock_guard<std::mutex> guard(mtx);
        return FkQueue<T>::empty();
    }

private:
    FkSafeQueue(const FkSafeQueue &o) = delete;

private:
    std::mutex mtx;
};

#endif //FK_BASE_FKSAFEQUEUE_H
