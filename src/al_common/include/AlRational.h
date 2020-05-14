/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALRATIONAL_H
#define HWVC_ANDROID_ALRATIONAL_H

#include "Object.h"

al_class AlRational al_extend Object {
public:
    AlRational();

    AlRational(int32_t num, int32_t den);

    AlRational(const AlRational &o);

    ~AlRational();

    AlRational &operator+(AlRational r);

    AlRational &operator=(const AlRational &o);

    float toFloat();

    double toDouble();

public:
    int32_t num; ///< Numerator(分子)
    int32_t den; ///< Denominator(分母)
};


#endif //HWVC_ANDROID_ALRATIONAL_H
