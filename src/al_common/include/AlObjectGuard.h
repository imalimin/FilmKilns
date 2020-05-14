/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_ALOBJECTGUARD_H
#define HWVC_ANDROID_ALOBJECTGUARD_H

#include "Object.h"

al_class AlObjectGuard al_extend Object {
public:
    AlObjectGuard(Object **obj);

    ~AlObjectGuard();

private:
    AlObjectGuard(const AlObjectGuard &o) : Object() {}

private:
    Object **obj = nullptr;
};


#endif //HWVC_ANDROID_ALOBJECTGUARD_H
