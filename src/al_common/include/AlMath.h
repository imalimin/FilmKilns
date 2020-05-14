/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALMATH_H
#define HWVC_ANDROID_ALMATH_H

#include <cmath>
#include "Object.h"

al_class AlMath al_extend Object {
public:
    static constexpr double PI = M_PI;

    static uint16_t swap16(uint16_t val);

    static uint32_t swap32(uint32_t val);

    /// 求解最大公约数
    /// \param a
    /// \param b
    /// \return
    static int32_t gcd(int32_t a, int32_t b);

    /// 求解最小公倍数
    /// \param a
    /// \param b
    /// \return
    static int32_t lcm(int32_t a, int32_t b);

    static int32_t random(int32_t min, int32_t max);

    static int32_t round32(double val);

    static int64_t round64(double val);

    static int32_t align16(int32_t val);

private:
    AlMath() : Object() {};

    AlMath(const AlMath &o) : Object() {};

    ~AlMath() {};
};


#endif //HWVC_ANDROID_ALMATH_H
