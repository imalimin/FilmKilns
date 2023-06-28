/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-5-1 14:54:28
*/

#include "FkPositionCompo.h"

FK_IMPL_CLASS_TYPE(FkPositionCompo, FkComponent)

FkPositionCompo::FkPositionCompo() : FkComponent(), loc(0, 0) {

}

FkPositionCompo::FkPositionCompo(const FkPositionCompo &o) : FkComponent(o), loc(o.loc) {

}

FkPositionCompo::~FkPositionCompo() {

}