/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlMath.h"
#include "TimeUtils.h"

uint16_t AlMath::swap16(uint16_t val) {
    return (val << 8) | (val >> 8);
}

uint32_t AlMath::swap32(uint32_t val) {
#if defined (__GNUC__) || defined(__clang__)
    return __builtin_bswap32(val);
#else
    return ((val & 0x000000FF) << 24)
             | ((val & 0x0000FF00) <<  8)
             | ((val & 0x00FF0000) >>  8)
             | ((val & 0xFF000000) >> 24);
#endif
}

int32_t AlMath::gcd(int32_t a, int32_t b) {
    while (a != b) {
        if (a > b) a = a - b;
        else b = b - a;
    }
    return a;
}

int32_t AlMath::lcm(int32_t a, int32_t b) {
    return a * b / gcd(a, b);
}

int32_t AlMath::random(int32_t min, int32_t max) {
    srand(static_cast<unsigned int>(TimeUtils::getCurrentTimeUS()));
    return rand() % (max - min + 1) + min;
}
