/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-20 23:30:00
*/

#include "FkMaterialEntity.h"

FK_IMPL_CLASS_TYPE(FkMaterialEntity, FkEntity)

std::shared_ptr<FkMaterialEntity> FkMaterialEntity::genEmpty() {
    return std::make_shared<FkMaterialEntity>(std::make_shared<FkMaterialCompo>(FK_ID_NONE));
}

FkMaterialEntity::FkMaterialEntity(std::shared_ptr<FkMaterialCompo> material) : FkEntity() {

    addComponent(material);
}

FkMaterialEntity::FkMaterialEntity(const FkMaterialEntity &o) : FkEntity(o) {

}

FkMaterialEntity::~FkMaterialEntity() {

}

std::shared_ptr<FkMaterialCompo> FkMaterialEntity::getMaterial() {
    return FK_FIND_COMPO(this, FkMaterialCompo);
}