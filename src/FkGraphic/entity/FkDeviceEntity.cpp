/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-26 11:47:54
*/

#include "FkDeviceEntity.h"
#include "FkBufCompo.h"
#include "FkFuncCompo.h"
#include "FkSizeCompo.h"
#include "FkFormatCompo.h"
#include "FkPositionCompo.h"

FK_IMPL_CLASS_TYPE(FkDeviceEntity, FkMaterialEntity)
FK_IMPL_CLASS_TYPE(FkBufDeviceEntity, FkDeviceEntity)
FK_IMPL_CLASS_TYPE(FkTexDeviceEntity, FkDeviceEntity)
FK_IMPL_CLASS_TYPE(FkScreenEntity, FkDeviceEntity)

FkDeviceEntity::FkDeviceEntity(std::shared_ptr<FkMaterialCompo> material)
        : FkMaterialEntity(material) {

}

FkDeviceEntity::FkDeviceEntity(const FkDeviceEntity &o) : FkMaterialEntity(o) {

}

FkDeviceEntity::~FkDeviceEntity() {

}

FkBufDeviceEntity::FkBufDeviceEntity(std::shared_ptr<FkBuffer> &buf)
        : FkDeviceEntity(std::make_shared<FkMaterialCompo>(FK_ID_NONE)) {

    addComponent(std::make_shared<FkBufCompo>(buf));
}

FkBufDeviceEntity::FkBufDeviceEntity(const FkBufDeviceEntity &o) : FkDeviceEntity(o) {

}

FkBufDeviceEntity::~FkBufDeviceEntity() {

}

std::shared_ptr<FkBuffer> FkBufDeviceEntity::buffer() {
    auto compo = FK_FIND_COMPO(this, FkBufCompo);
    if (compo) {
        return compo->buf;
    }
    return nullptr;
}

void FkBufDeviceEntity::finish() {
    auto compo = FK_FIND_COMPO(this, FkFuncCompo);
    if (compo) {
        return compo->func();
    }
}

void FkBufDeviceEntity::setPosition(int32_t x, int32_t y) {
    auto compo = std::make_shared<FkPositionCompo>();
    compo->loc.x = x;
    compo->loc.y = y;
    addComponent(compo);
}

FkIntVec2 FkBufDeviceEntity::getPosition() {
    auto compo = FK_FIND_COMPO(this, FkPositionCompo);
    if (compo) {
        return compo->loc;
    }
    return FkIntVec2(0, 0);
}

void FkBufDeviceEntity::setSize(FkSize size) {
    addComponent(std::make_shared<FkSizeCompo>(size));
}

FkSize FkBufDeviceEntity::getSize() {
    auto compo = FK_FIND_COMPO(this, FkSizeCompo);
    if (compo) {
        return compo->size;
    }
    return FkSize(0, 0);
}

FkTexDeviceEntity::FkTexDeviceEntity(std::shared_ptr<FkMaterialCompo> material)
        : FkDeviceEntity(material) {

}

FkTexDeviceEntity::FkTexDeviceEntity(const FkTexDeviceEntity &o) : FkDeviceEntity(o) {

}

FkTexDeviceEntity::~FkTexDeviceEntity() {

}

void FkTexDeviceEntity::finish() {
    auto compo = FK_FIND_COMPO(this, FkFuncCompo);
    if (compo) {
        return compo->func();
    }
}

std::shared_ptr<FkTexCompo> FkTexDeviceEntity::tex() {
    return FK_FIND_COMPO(this, FkTexCompo);
}

FkSize FkTexDeviceEntity::size() {
    auto compo = FK_FIND_COMPO(this, FkSizeCompo);
    if (compo) {
        return compo->size;
    }
    return FkSize(0, 0);
}

FkColor::kFormat FkTexDeviceEntity::format() {
    auto compo = FK_FIND_COMPO(this, FkFormatCompo);
    if (compo) {
        return compo->fmt;
    }
    return FkColor::kFormat::NONE;
}

FkScreenEntity::FkScreenEntity()
        : FkDeviceEntity(std::make_shared<FkMaterialCompo>(FK_ID_NONE)) {

}

FkScreenEntity::FkScreenEntity(const FkTexDeviceEntity &o) : FkDeviceEntity(o) {

}

FkScreenEntity::~FkScreenEntity() {

}

FkSize FkScreenEntity::size() {
    auto compo = FK_FIND_COMPO(this, FkSizeCompo);
    if (compo) {
        return compo->size;
    }
    return FkSize(0, 0);
}
