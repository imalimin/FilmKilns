/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALRUNNABLE_H
#define HWVC_ANDROID_ALRUNNABLE_H

#include "Object.h"
#include <functional>

al_def_class AlRunnable al_extend Object {
public:
    static AlRunnable *run(const std::function<void(Object *obj)> &runnable);

    static AlRunnable *runEmptyArgs(const std::function<void()> &runnable);

public:
    AlRunnable(std::function<void(Object *obj)> runnable);

    AlRunnable(const AlRunnable &o);

    virtual ~AlRunnable();

    void operator()(Object *obj);

private:
    std::function<void(Object *obj)> runnable = nullptr;
};


#endif //HWVC_ANDROID_ALRUNNABLE_H
