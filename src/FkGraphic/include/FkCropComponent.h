/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2024-03-09 16:40:44
*/

#ifndef FK_GRAPHIC_FKCROPCOMPONENT_H
#define FK_GRAPHIC_FKCROPCOMPONENT_H

#include "FkComponent.h"
#include "FkRect.h"

FK_SUPER_CLASS(FkCropComponent, FkComponent) {
FK_DEF_CLASS_TYPE_FUNC(FkCropComponent)

public:
    FkCropComponent(const FkIntRect &rect);

    FkCropComponent(const FkCropComponent &o);

    virtual ~FkCropComponent();

public:
    FkIntRect rect;
};

#endif //FK_GRAPHIC_FKCROPCOMPONENT_H