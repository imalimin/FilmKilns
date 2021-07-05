/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_BASE_FKFLOATVEC3_H
#define FK_BASE_FKFLOATVEC3_H

#include "FkVec3.h"

FK_CLASS FkFloatVec3 FK_EXTEND FkVec3<float> {
public:
    FkFloatVec3(float x, float y, float z);

    FkFloatVec3(const FkFloatVec3 &o);

    virtual ~FkFloatVec3();
};

#endif //FK_BASE_FKFLOATVEC3_H