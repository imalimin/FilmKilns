/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "AlFuncWrapper.h"

AlFuncWrapper *AlFuncWrapper::wrap(std::function<void()> func) {
    return new AlFuncWrapper(func);
}

AlFuncWrapper::AlFuncWrapper(std::function<void()> func) : Object(), func(func) {

}

AlFuncWrapper::~AlFuncWrapper() {
    func = nullptr;
}

void AlFuncWrapper::invoke() {
    if (func) {
        func();
    }
}