/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: ${author}
* CREATE TIME: 2022-2-25 13:23:24
*/

#include "FkColorCompo.h"

FK_IMPL_CLASS_TYPE(FkColorCompo, FkComponent)

FkColorCompo::FkColorCompo(FkColor color) : FkComponent(), color(color) {

}

FkColorCompo::FkColorCompo(const FkColorCompo &o) : FkComponent(o), color(o.color) {

}

FkColorCompo::~FkColorCompo() {

}