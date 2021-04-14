/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKCOLORCOMPONENT_H
#define FK_GRAPHIC_FKCOLORCOMPONENT_H

#include "FkGraphicComponent.h"
#include "FkColor.h"
#include "FkSize.h"

FK_CLASS FkColorComponent FK_EXTEND FkGraphicComponent {
public:
    FkColorComponent();

    FkColorComponent(const FkColorComponent &o);

    virtual ~FkColorComponent();

public:
    FkColor color;
    FkSize size;
};

#endif //FK_GRAPHIC_FKCOLORCOMPONENT_H
