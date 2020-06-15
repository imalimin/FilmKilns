/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALVECTOR_H
#define HWVC_ANDROID_ALVECTOR_H

#include "Object.h"
#include <vector>

template<class T>
AL_CLASS AlVector AL_EXTEND Object {
public:
    typedef typename std::vector<T>::iterator iterator;
    AlVector() : Object() {

    }

    virtual ~AlVector() {
        mVector.clear();
    }

    iterator begin() {
        return mVector.begin();
    }

    iterator end() {
        return mVector.end();
    }

    void push_back(const T &o) {
        mVector.push_back(o);
    }

    size_t size() {
        return mVector.size();
    }

    bool empty() {
        return mVector.empty();
    }

private:
    AlVector(const AlVector &o) : Object() {

    }

private:
    std::vector<T> mVector;
};

#endif //HWVC_ANDROID_ALVECTOR_H
