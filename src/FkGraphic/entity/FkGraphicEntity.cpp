/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicEntity.h"

FkGraphicEntity::FkGraphicEntity() : FkObject() {
    FK_MARK_SUPER

}

FkGraphicEntity::FkGraphicEntity(const FkGraphicEntity &o) : FkObject() {
    FK_MARK_SUPER

}

FkGraphicEntity::~FkGraphicEntity() {

}

FkResult FkGraphicEntity::addComponent(std::shared_ptr<FkGraphicComponent> comp) {
    components.emplace_back(comp);
    return FK_OK;
}
