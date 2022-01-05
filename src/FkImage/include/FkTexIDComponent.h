/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKTEXCOMPONENT_H
#define FK_GRAPHIC_FKTEXCOMPONENT_H

#include "FkBufferComponent.h"
#include "FkGraphicTexture.h"

FK_CLASS FkTexIDComponent FK_EXTEND FkBufferComponent {
public:
    FkTexIDComponent();

    FkTexIDComponent(const FkTexIDComponent &o);

    virtual ~FkTexIDComponent();

public:
    FkID id = FK_ID_NONE;
};


#endif //FK_GRAPHIC_FKTEXCOMPONENT_H
