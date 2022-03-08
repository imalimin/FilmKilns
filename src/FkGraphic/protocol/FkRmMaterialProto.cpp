/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-3-9 0:04:41
*/

#include "FkRmMaterialProto.h"

FkRmMaterialProto::FkRmMaterialProto() : FkMaterialProto(nullptr) {
    FK_MARK_SUPER
}

FkRmMaterialProto::FkRmMaterialProto(std::shared_ptr<FkMaterialCompo> &material)
        : FkMaterialProto(material) {
    FK_MARK_SUPER
}

FkRmMaterialProto::FkRmMaterialProto(const FkRmMaterialProto &o)
        : FkMaterialProto(o) {
    FK_MARK_SUPER
}

FkRmMaterialProto::~FkRmMaterialProto() {

}