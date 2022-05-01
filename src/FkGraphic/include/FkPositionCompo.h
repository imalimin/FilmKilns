/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-5-1 14:54:28
*/

#ifndef FK_GRAPHIC_FKLOCATIONCOMPO_H
#define FK_GRAPHIC_FKLOCATIONCOMPO_H

#include "FkComponent.h"
#include "FkIntVec2.h"

FK_SUPER_CLASS(FkPositionCompo, FkComponent) {
FK_DEF_CLASS_TYPE_FUNC(FkPositionCompo)

public:
    FkPositionCompo();

    FkPositionCompo(const FkPositionCompo &o);

    virtual ~FkPositionCompo();

public:
    FkIntVec2 loc;
};

#endif //FK_GRAPHIC_FKLOCATIONCOMPO_H