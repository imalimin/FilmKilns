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

FkDeviceEntity::FkDeviceEntity(std::shared_ptr<FkMaterialCompo> material)
        : FkMaterialEntity(material) {
    FK_MARK_SUPER
}

FkDeviceEntity::FkDeviceEntity(const FkDeviceEntity &o) : FkMaterialEntity(o) {
    FK_MARK_SUPER
}

FkDeviceEntity::~FkDeviceEntity() {

}

FkBufDeviceEntity::FkBufDeviceEntity(std::shared_ptr<FkBuffer> &buf)
        : FkDeviceEntity(std::make_shared<FkMaterialCompo>(FK_ID_NONE)) {
    FK_MARK_SUPER
    addComponent(std::make_shared<FkBufCompo>(buf));
}

FkBufDeviceEntity::FkBufDeviceEntity(const FkBufDeviceEntity &o) : FkDeviceEntity(o) {
    FK_MARK_SUPER
}

FkBufDeviceEntity::~FkBufDeviceEntity() {

}

std::shared_ptr<FkBuffer> FkBufDeviceEntity::buffer() {
    auto compo = findComponent<FkBufCompo>();
    if (compo) {
        return compo->buf;
    }
    return nullptr;
}

void FkBufDeviceEntity::finish() {
    auto compo = findComponent<FkFuncCompo>();
    if (compo) {
        return compo->func();
    }
}

FkTexDeviceEntity::FkTexDeviceEntity(std::shared_ptr<FkMaterialCompo> material)
        : FkDeviceEntity(material) {
    FK_MARK_SUPER
}

FkTexDeviceEntity::FkTexDeviceEntity(const FkTexDeviceEntity &o) : FkDeviceEntity(o) {
    FK_MARK_SUPER
}

FkTexDeviceEntity::~FkTexDeviceEntity() {

}

void FkTexDeviceEntity::finish() {
    auto compo = findComponent<FkFuncCompo>();
    if (compo) {
        return compo->func();
    }
}

std::shared_ptr<FkTexCompo> FkTexDeviceEntity::tex() {
    return findComponent<FkTexCompo>();
}

FkSize FkTexDeviceEntity::size() {
    auto compo = findComponent<FkSizeCompo>();
    if (compo) {
        return compo->size;
    }
    return FkSize(0, 0);
}

FkColor::kFormat FkTexDeviceEntity::format() {
    auto compo = findComponent<FkFormatCompo>();
    if (compo) {
        return compo->fmt;
    }
    return FkColor::kFormat::NONE;
}

FkScreenEntity::FkScreenEntity()
        : FkDeviceEntity(std::make_shared<FkMaterialCompo>(FK_ID_NONE)) {
    FK_MARK_SUPER
}

FkScreenEntity::FkScreenEntity(const FkTexDeviceEntity &o) : FkDeviceEntity(o) {
    FK_MARK_SUPER
}

FkScreenEntity::~FkScreenEntity() {

}
