/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKSCALETYPECOMPONENT_H
#define FK_GRAPHIC_FKSCALETYPECOMPONENT_H

#include "FkGraphicComponent.h"
#include "FkDefinition.h"

FK_CLASS FkScaleTypeComponent FK_EXTEND FkGraphicComponent {
public:
    FkScaleTypeComponent();

    FkScaleTypeComponent(const FkScaleTypeComponent &o);

    virtual ~FkScaleTypeComponent();

public:
    kScaleType value;
};

#endif //FK_GRAPHIC_FKSCALETYPECOMPONENT_H