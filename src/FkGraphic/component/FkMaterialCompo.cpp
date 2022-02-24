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

FkMaterialCompo::FkMaterialCompo(FkID id) : FkGraphicComponent(), id(id) {
    FK_MARK_SUPER
}

FkMaterialCompo::FkMaterialCompo(const FkMaterialCompo &o) : FkGraphicComponent(o), id(o.id) {
    FK_MARK_SUPER
}

FkMaterialCompo::~FkMaterialCompo() {

}

bool FkMaterialCompo::isUseless() {
    return id == FK_ID_NONE;
}
