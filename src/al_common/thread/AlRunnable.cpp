/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlRunnable.h"

AlRunnable *AlRunnable::run(const std::function<void(Object *obj)> &runnable) {
    return new AlRunnable(runnable);
}

AlRunnable *AlRunnable::runEmptyArgs(const std::function<void()> &runnable) {
    return run([runnable](Object *o) {
        runnable();
    });
}

AlRunnable::AlRunnable(std::function<void(Object *obj)> runnable) : Object(), runnable(std::move(runnable)) {

}

AlRunnable::AlRunnable(const AlRunnable &o) : Object(), runnable(o.runnable) {

}

AlRunnable::~AlRunnable() {
    runnable = nullptr;
}

void AlRunnable::operator()(Object *obj) {
    if (runnable) {
        runnable(obj);
        runnable = nullptr;
    }
}
