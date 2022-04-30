/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: ${author}
* CREATE TIME: 2022-2-25 13:23:15
*/

#include "FkSizeCompo.h"

FK_IMPL_CLASS_TYPE(FkSizeCompo, FkComponent)

FkSizeCompo::FkSizeCompo(FkSize size) : FkComponent(), size(size) {

}

FkSizeCompo::FkSizeCompo(const FkSizeCompo &o) : FkComponent(o), size(o.size) {

}

FkSizeCompo::~FkSizeCompo() {

}