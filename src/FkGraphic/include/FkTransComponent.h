/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKTRANSCOMPONENT_H
#define FK_GRAPHIC_FKTRANSCOMPONENT_H

#include "FkGraphicComponent.h"
#include "FkVec2.h"

FK_CLASS FkTransComponent FK_EXTEND FkGraphicComponent {
public:
    FkTransComponent();

    FkTransComponent(const FkTransComponent &o);

    virtual ~FkTransComponent();

public:
    FkVec2 position;
};


#endif //FK_GRAPHIC_FKTRANSCOMPONENT_H
