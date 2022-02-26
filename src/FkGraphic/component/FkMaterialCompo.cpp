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

FkMaterialCompo::FkMaterialCompo(FkID id) : FkComponent(), _id(id) {
    FK_MARK_SUPER
}

FkMaterialCompo::FkMaterialCompo(const FkMaterialCompo &o) : FkComponent(o), _id(o._id) {
    FK_MARK_SUPER
}

FkMaterialCompo::~FkMaterialCompo() {

}

bool FkMaterialCompo::isUseless() {
    return _id == FK_ID_NONE;
}

FkID FkMaterialCompo::id() const {
    return _id;
}
