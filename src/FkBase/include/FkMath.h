/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-3-27 17:09:52
*/

#ifndef FK_BASE_FKMATH_H
#define FK_BASE_FKMATH_H

#include "FkObject.h"
#include "FkIntVec2.h"

class FkMath {
public:
    virtual ~FkMath();

    static float distance(FkIntVec2 &p0, FkIntVec2 &p1);

private:
    FkMath() = delete;

    FkMath(const FkMath &o) = delete;
};

#endif //FK_BASE_FKMATH_H