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
