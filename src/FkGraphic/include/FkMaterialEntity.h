/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-20 23:30:00
*/

#ifndef FK_GRAPHIC_FKMATERIALENTITY_H
#define FK_GRAPHIC_FKMATERIALENTITY_H

#include "FkEntity.h"

FK_CLASS FkMaterialEntity FK_EXTEND FkEntity {
public:
    FkMaterialEntity();

    FkMaterialEntity(const FkMaterialEntity &o);

    virtual ~FkMaterialEntity();
};

#endif //FK_GRAPHIC_FKMATERIALENTITY_H