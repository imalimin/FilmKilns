/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICCOMPONENT_H
#define FK_GRAPHIC_FKGRAPHICCOMPONENT_H

#include "FkComponent.h"

FK_SUPER_CLASS(FkGraphicComponent, FkComponent) {
FK_DEF_CLASS_TYPE_FUNC(FkGraphicComponent)

public:
    FkGraphicComponent();

    FkGraphicComponent(const FkGraphicComponent &o);

    virtual ~FkGraphicComponent();
};


#endif //FK_GRAPHIC_FKGRAPHICCOMPONENT_H
