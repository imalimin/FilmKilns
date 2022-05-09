/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2022-05-08 21:15:50
*/

#ifndef FK_BASE_FKDOUBLEVEC2_H
#define FK_BASE_FKDOUBLEVEC2_H

#include "FkVec2.h"

FK_SUPER_CLASS(FkDoubleVec2, FkVec2)<double> {
FK_DEF_CLASS_TYPE_FUNC(FkDoubleVec2)

public:
    FkDoubleVec2(double x, double y);

    FkDoubleVec2(const FkDoubleVec2 &o);

    virtual ~FkDoubleVec2();

    FkDoubleVec2 operator/(double len) const;

    FkDoubleVec2 operator*(double len) const;

    FkDoubleVec2 operator-(const FkDoubleVec2 &other) const;

    FkDoubleVec2 operator+(const FkDoubleVec2 &other) const;
};

#endif //FK_BASE_FKDOUBLEVEC2_H