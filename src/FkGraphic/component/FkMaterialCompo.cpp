/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-24 23:50:22
*/

#include "FkMaterialCompo.h"

FK_IMPL_CLASS_TYPE(FkMaterialCompo, FkComponent)

FkMaterialCompo::FkMaterialCompo(FkID id) : FkComponent(), _id(id) {

}

FkMaterialCompo::FkMaterialCompo(const FkMaterialCompo &o) : FkComponent(o), _id(o._id) {

}

FkMaterialCompo::~FkMaterialCompo() {

}

bool FkMaterialCompo::isInvalid() {
    return _id == FK_ID_NONE;
}

FkID FkMaterialCompo::id() const {
    return _id;
}
