//
// Created by limin on 2019/12/19.
//

#ifndef HWVC_ANDROID_ALRUNNABLE_H
#define HWVC_ANDROID_ALRUNNABLE_H

#include "Object.h"
#include <functional>

al_class(AlRunnable) {
public:
    AlRunnable(std::function<void(Object *obj)> runnable);

    virtual ~AlRunnable();

    void operator()(Object *obj);

private:
    AlRunnable(const AlRunnable &o) : Object() {}

    std::function<void(Object *obj)> runnable = nullptr;
};


#endif //HWVC_ANDROID_ALRUNNABLE_H
