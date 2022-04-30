/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKSHAPECOMPONENT_H
#define FK_GRAPHIC_FKSHAPECOMPONENT_H

#include "FkGraphicComponent.h"
#include "FkShape.h"

FK_SUPER_CLASS(FkShapeComponent, FkGraphicComponent) {
FK_DEF_CLASS_TYPE_FUNC(FkShapeComponent)

public:
    FkShapeComponent();

    FkShapeComponent(const FkShapeComponent &o);

    virtual ~FkShapeComponent();

public:
    FkShape::kType type = FkShape::kType::RECT;
};

#endif //FK_GRAPHIC_FKSHAPECOMPONENT_H