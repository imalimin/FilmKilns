/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_BASE_FKFLOATVEC2_H
#define FK_BASE_FKFLOATVEC2_H

#include "FkVec2.h"

FK_CLASS FkFloatVec2 FK_EXTEND FkVec2<float> {
public:
    FkFloatVec2(float x, float y);

    FkFloatVec2(const FkFloatVec2 &o);

    virtual ~FkFloatVec2();

    virtual bool isNan();
};


#endif //FK_BASE_FKFLOATVEC2_H
