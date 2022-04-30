/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-4-27 0:32:56
*/

#ifndef FK_GRAPHIC_FKFLOATVEC2COMPO_H
#define FK_GRAPHIC_FKFLOATVEC2COMPO_H

#include "FkComponent.h"
#include "FkFloatVec2.h"

FK_SUPER_CLASS(FkFloatVec2Compo, FkComponent) {
FK_DEF_CLASS_TYPE_FUNC(FkFloatVec2Compo)

public:
    FkFloatVec2Compo(float x, float y);

    FkFloatVec2Compo(const FkFloatVec2Compo &o);

    virtual ~FkFloatVec2Compo();

public:
    FkFloatVec2 vec;
};

#endif //FK_GRAPHIC_FKFLOATVEC2COMPO_H