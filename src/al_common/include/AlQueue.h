/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALQUEUE_H
#define HWVC_ANDROID_ALQUEUE_H

#include "Object.h"
#include <queue>

template<class T>
AL_CLASS AlQueue AL_EXTEND Object {
public:
    AlQueue() : Object() {

    }

    virtual ~AlQueue() {

    }

    virtual void push(const T &o) {
        mQueue.push(o);
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
    AlQueue(const AlQueue &o) : Object() {

    }

private:
    std::queue<T> mQueue;
};
#endif //HWVC_ANDROID_ALQUEUE_H
