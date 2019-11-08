/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_ALFUNCWRAPPER_H
#define HWVC_ANDROID_ALFUNCWRAPPER_H

#include "Object.h"
#include "functional"

al_class(AlFuncWrapper) {
public:
    static AlFuncWrapper *wrap(std::function<void()> func);

    ~AlFuncWrapper();

    void invoke();

private:
    AlFuncWrapper(std::function<void()> func);

    AlFuncWrapper(AlFuncWrapper &e) {};
private:
    std::function<void()> func = nullptr;
};


#endif //HWVC_ANDROID_ALFUNCWRAPPER_H
