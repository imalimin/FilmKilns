/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_BASE_FKQUEUE_H
#define FK_BASE_FKQUEUE_H

#include "FkObject.h"
#include <queue>

FK_SUPER_TEMPLATE_CLASS(FkQueue, FkObject, class T) {
FK_DEF_CLASS_TYPE_FUNC(FkQueue)

public:
    FkQueue() : FkObject() {}

    virtual ~FkQueue() {}

    virtual void push(const T &o) {
        mQueue.push(o);
    }

    virtual T top() {
        T o = mQueue.front();
        return o;
    }

    virtual T pop() {
        T o = mQueue.front();
        mQueue.pop();
        return o;
    }

    virtual size_t size() {
        return mQueue.size();
    }

    virtual bool empty() {
        return 0 == mQueue.size();
    }

private:
    FkQueue(const FkQueue &o) = delete;

private:
    std::queue <T> mQueue;
};


#endif //FK_BASE_FKQUEUE_H
