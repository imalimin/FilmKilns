/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-3-9 0:08:13
*/

#include "FkMaterialProto.h"

FK_IMPL_CLASS_TYPE(FkMaterialProto, FkProtocol)

FkMaterialProto::FkMaterialProto(std::shared_ptr<FkMaterialCompo> material)
        : FkProtocol(), material(material) {

}

FkMaterialProto::FkMaterialProto(const FkMaterialProto &o)
        : FkProtocol(o), material(o.material) {

}

FkMaterialProto::~FkMaterialProto() {

}