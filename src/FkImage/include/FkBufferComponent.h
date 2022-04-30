/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKBUFFERCOMPONENT_H
#define FK_GRAPHIC_FKBUFFERCOMPONENT_H

#include "FkGraphicComponent.h"

FK_SUPER_CLASS(FkBufferComponent, FkGraphicComponent) {
FK_DEF_CLASS_TYPE_FUNC(FkBufferComponent)

public:
    FkBufferComponent();

    FkBufferComponent(const FkBufferComponent &o);

    virtual ~FkBufferComponent();

};


#endif //FK_GRAPHIC_FKBUFFERCOMPONENT_H
