/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-3-5 20:44:47
*/

#ifndef FK_GRAPHIC_FKRENDERENTITY_H
#define FK_GRAPHIC_FKRENDERENTITY_H

#include "FkEntity.h"

FK_SUPER_CLASS(FkRenderEntity, FkEntity) {
FK_DEF_CLASS_TYPE_FUNC(FkRenderEntity)

public:
    FkRenderEntity();

    FkRenderEntity(const FkRenderEntity &o);

    virtual ~FkRenderEntity();
};

#endif //FK_GRAPHIC_FKRENDERENTITY_H