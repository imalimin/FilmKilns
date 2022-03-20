/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKPOINTCOMPONENT_H
#define FK_GRAPHIC_FKPOINTCOMPONENT_H

#include "FkGraphicComponent.h"
#include "FkIntVec2.h"
#include "FkColor.h"

FK_CLASS FkPointComponent FK_EXTEND FkGraphicComponent {
public:
    FkPointComponent();

    FkPointComponent(const FkPointComponent &o);

    virtual ~FkPointComponent();

public:
    FkIntVec2 value;
    FkColor color;
};

#endif //FK_GRAPHIC_FKPOINTCOMPONENT_H