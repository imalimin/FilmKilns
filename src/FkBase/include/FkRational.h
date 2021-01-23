/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_BASE_FKRATIONAL_H
#define FK_BASE_FKRATIONAL_H

#include "FkObject.h"
#include "FkRational.h"

FK_CLASS FkRational FK_EXTEND FkObject {
public:
    FkRational();

    FkRational(int32_t num, int32_t den);

    FkRational(const FkRational &o);

    ~FkRational();

    FkRational &operator+(FkRational r);

    FkRational &operator=(const FkRational &o);

    float toFloat();

    double toDouble();

public:
    int32_t num; ///< Numerator(分子)
    int32_t den; ///< Denominator(分母)
};


#endif //FK_BASE_FKRATIONAL_H
