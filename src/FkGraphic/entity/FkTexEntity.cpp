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
#include "FkSizeCompo.h"
#include "FkFormatCompo.h"

FK_IMPL_CLASS_TYPE(FkTexEntity, FkMaterialEntity)

FkTexEntity::FkTexEntity(std::shared_ptr<FkMaterialCompo> &material)
        : FkMaterialEntity(material), scale(1.0f, 1.0f, 1.0f) {

}

FkTexEntity::FkTexEntity(std::shared_ptr<FkMaterialCompo> &material,
                         std::shared_ptr<FkTexCompo> &tex)
        : FkMaterialEntity(material), scale(1.0f, 1.0f, 1.0f) {
    addComponent(tex);
}

FkTexEntity::FkTexEntity(const FkTexEntity &o) : FkMaterialEntity(o), scale(o.scale) {

}

FkTexEntity::FkTexEntity(const FkMaterialEntity &o) : FkMaterialEntity(o), scale(1.0f, 1.0f, 1.0f) {
}

FkTexEntity::~FkTexEntity() {

}

std::shared_ptr<FkTexCompo> FkTexEntity::tex() {
    return FK_FIND_COMPO(this, FkTexCompo);
}

std::shared_ptr<FkFboCompo> FkTexEntity::fbo() {
    return FK_FIND_COMPO(this, FkFboCompo);
}

FkSize FkTexEntity::size() {
    auto compo = FK_FIND_COMPO(this, FkSizeCompo);
    if (compo) {
        return compo->size;
    }
    return FkSize(0, 0);
}

FkColor::kFormat FkTexEntity::format() {
    auto compo = FK_FIND_COMPO(this, FkFormatCompo);
    if (compo) {
        return compo->fmt;
    }
    return FkColor::kFormat::NONE;
}
