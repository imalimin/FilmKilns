/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_BASE_FKINTVEC2_H
#define FK_BASE_FKINTVEC2_H

#include "FkVec2.h"

FK_SUPER_TEMPLATE_CLASS_IMPL(FkIntVec2, FkVec2)<int32_t> {
FK_DEF_CLASS_TYPE_FUNC(FkIntVec2)

public:
    FkIntVec2(int32_t x, int32_t y);

    FkIntVec2(const FkIntVec2 &o);

    virtual ~FkIntVec2();

    virtual FkIntVec2 operator+(const FkIntVec2 &vec);

    virtual FkIntVec2 operator-(const FkIntVec2 &vec);

};


#endif //FK_BASE_FKINTVEC2_H
