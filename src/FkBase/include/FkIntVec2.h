/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_BASE_FKINTVEC2_H
#define FK_BASE_FKINTVEC2_H

#include "FkVec2.h"

FK_CLASS FkIntVec2 FK_EXTEND FkVec2<int32_t>{
public:
    FkIntVec2(int32_t x, int32_t y);

    FkIntVec2(const FkIntVec2 &o);

    virtual ~FkIntVec2();

};


#endif //FK_BASE_FKINTVEC2_H
