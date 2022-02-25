/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: ${author}
* CREATE TIME: 2022-2-25 13:24:09
*/

#include "FkTexEntity.h"

FkTexEntity::FkTexEntity() : FkMaterialEntity() {
    FK_MARK_SUPER
}

FkTexEntity::FkTexEntity(const FkTexEntity &o) : FkMaterialEntity(o), size(o.size) {
    FK_MARK_SUPER
}

FkTexEntity::~FkTexEntity() {

}

std::shared_ptr<FkTexCompo> FkTexEntity::tex() {
    return findComponent<FkTexCompo>();
}

FkSize FkTexEntity::size() {
    auto compo = findComponent<FkSizeCompo>();
    if (compo) {
        return compo->size;
    }
    return FkSize(0, 0);
}