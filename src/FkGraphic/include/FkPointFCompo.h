/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-3-19 19:59:27
*/

#ifndef FK_GRAPHIC_FKPOINTFCOMPO_H
#define FK_GRAPHIC_FKPOINTFCOMPO_H

#include "FkComponent.h"
#include "FkFloatVec3.h"
#include "FkColor.h"

FK_CLASS FkPointFCompo FK_EXTEND FkComponent {
public:
    FkPointFCompo();

    FkPointFCompo(const FkPointFCompo &o);

    virtual ~FkPointFCompo();

public:
    FkFloatVec3 value;
    FkColor color;
    int32_t size = 0;
};

#endif //FK_GRAPHIC_FKPOINTFCOMPO_H