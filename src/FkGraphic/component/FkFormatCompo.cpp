/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-26 0:23:57
*/

#include "FkFormatCompo.h"

FK_IMPL_CLASS_TYPE(FkFormatCompo, FkComponent)

FkFormatCompo::FkFormatCompo(FkColor::kFormat fmt) : FkComponent(), fmt(fmt) {

}

FkFormatCompo::FkFormatCompo(const FkFormatCompo &o) : FkComponent(o), fmt(o.fmt) {

}

FkFormatCompo::~FkFormatCompo() {

}