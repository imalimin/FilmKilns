/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALMATH_H
#define HWVC_ANDROID_ALMATH_H

#include "Object.h"

al_class(AlMath) {
public:
    static uint16_t swap16(uint16_t val);

    static uint32_t swap32(uint32_t val);

private:
    AlMath() : Object() {};

    AlMath(const AlMath &o) : Object() {};

    ~AlMath() {};
};


#endif //HWVC_ANDROID_ALMATH_H
