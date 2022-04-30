/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKSCALECOMPONENT_H
#define FK_GRAPHIC_FKSCALECOMPONENT_H

#include "FkGraphicComponent.h"
#include "FkFloatVec3.h"

FK_SUPER_CLASS(FkScaleComponent, FkGraphicComponent) {
FK_DEF_CLASS_TYPE_FUNC(FkScaleComponent)

public:
    FkScaleComponent();

    FkScaleComponent(FkFloatVec3 value);

    FkScaleComponent(const FkScaleComponent &o);

    virtual ~FkScaleComponent();

public:
    FkFloatVec3 value;
};


#endif //FK_GRAPHIC_FKSCALECOMPONENT_H
