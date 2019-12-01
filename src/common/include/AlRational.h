/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALRATIONAL_H
#define HWVC_ANDROID_ALRATIONAL_H

#include "Object.h"

al_class(AlRational) {
public:
    AlRational(int32_t num, int32_t den);

    AlRational(const AlRational &o);

    ~AlRational();

public:
    int32_t num; ///< Numerator
    int32_t den; ///< Denominator
};


#endif //HWVC_ANDROID_ALRATIONAL_H
