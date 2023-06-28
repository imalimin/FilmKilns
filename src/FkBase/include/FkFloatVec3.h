/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_BASE_FKFLOATVEC3_H
#define FK_BASE_FKFLOATVEC3_H

#include "FkVec3.h"

FK_SUPER_TEMPLATE_CLASS_IMPL(FkFloatVec3, FkVec3)<float> {
FK_DEF_CLASS_TYPE_FUNC(FkFloatVec3)

public:
    FkFloatVec3(float x, float y, float z);

    FkFloatVec3(const FkFloatVec3 &o);

    virtual ~FkFloatVec3();
};

#endif //FK_BASE_FKFLOATVEC3_H