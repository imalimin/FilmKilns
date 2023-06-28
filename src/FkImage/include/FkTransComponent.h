/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKTRANSCOMPONENT_H
#define FK_GRAPHIC_FKTRANSCOMPONENT_H

#include "FkGraphicComponent.h"
#include "FkDoubleVec2.h"

FK_SUPER_CLASS(FkTransComponent, FkGraphicComponent) {
FK_DEF_CLASS_TYPE_FUNC(FkTransComponent)

public:
    FkTransComponent();

    FkTransComponent(const FkTransComponent &o);

    virtual ~FkTransComponent();

public:
    FkDoubleVec2 value;
};


#endif //FK_GRAPHIC_FKTRANSCOMPONENT_H
