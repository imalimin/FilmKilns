/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKROTATECOMPONENT_H
#define FK_GRAPHIC_FKROTATECOMPONENT_H

#include "FkGraphicComponent.h"
#include "FkRational.h"

FK_SUPER_CLASS(FkRotateComponent, FkGraphicComponent) {
FK_DEF_CLASS_TYPE_FUNC(FkRotateComponent)

public:
    FkRotateComponent();

    FkRotateComponent(const FkRotateComponent &o);

    virtual ~FkRotateComponent();

public:
    ///弧度表示，顺时针方向为正向(CW)，r=rotation*PI
    FkRational value;
};


#endif //FK_GRAPHIC_FKROTATECOMPONENT_H
