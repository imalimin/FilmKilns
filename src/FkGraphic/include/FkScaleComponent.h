/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKSCALECOMPONENT_H
#define FK_GRAPHIC_FKSCALECOMPONENT_H

#include "FkGraphicComponent.h"
#include "FkFloatVec2.h"

FK_CLASS FkScaleComponent FK_EXTEND FkGraphicComponent {
public:
    FkScaleComponent();

    FkScaleComponent(const FkScaleComponent &o);

    virtual ~FkScaleComponent();

public:
    FkFloatVec2 scale;
};


#endif //FK_GRAPHIC_FKSCALECOMPONENT_H
