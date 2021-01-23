/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKMATERIALCOMPONENT_H
#define FK_GRAPHIC_FKMATERIALCOMPONENT_H

#include "FkGraphicComponent.h"

FK_CLASS FkMaterialComponent FK_EXTEND FkGraphicComponent {
public:
    FkMaterialComponent();

    FkMaterialComponent(const FkMaterialComponent &o);

    virtual ~FkMaterialComponent();

};


#endif //FK_GRAPHIC_FKMATERIALCOMPONENT_H
