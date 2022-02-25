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

FkMaterialEntity::FkMaterialEntity() : FkEntity() {
    FK_MARK_SUPER
}

FkMaterialEntity::FkMaterialEntity(const FkMaterialEntity &o) : FkEntity(o) {
    FK_MARK_SUPER
}

FkMaterialEntity::~FkMaterialEntity() {

}

std::shared_ptr<FkMaterialCompo> FkMaterialEntity::getMaterial() {
    return findComponent<FkMaterialCompo>();
}