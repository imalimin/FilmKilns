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
#include "FkMaterialCompo.h"

FK_SUPER_CLASS(FkMaterialEntity, FkEntity) {
FK_DEF_CLASS_TYPE_FUNC(FkMaterialEntity)

public:
    static std::shared_ptr<FkMaterialEntity> genEmpty();

public:
    FkMaterialEntity(std::shared_ptr<FkMaterialCompo> material);

    FkMaterialEntity(const FkMaterialEntity &o);

    virtual ~FkMaterialEntity();

    std::shared_ptr<FkMaterialCompo> getMaterial();

};

#endif //FK_GRAPHIC_FKMATERIALENTITY_H