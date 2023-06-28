/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-3-19 19:59:27
*/

#include "FkPointFCompo.h"

FK_IMPL_CLASS_TYPE(FkPointFCompo, FkComponent)

FkPointFCompo::FkPointFCompo()
        : FkComponent(), value(0.0f, 0.0f, 0.0f), color(FkColor::white()) {

}

FkPointFCompo::FkPointFCompo(const FkPointFCompo &o)
        : FkComponent(o), value(o.value), color(o.color), size(o.size) {

}

FkPointFCompo::~FkPointFCompo() {

}