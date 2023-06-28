/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-3-13 13:21:51
*/

#include "FkLayerSetTransProto.h"

FK_IMPL_CLASS_TYPE(FkLayerSetTransProto, FkIntVec2Proto)

FkLayerSetTransProto::FkLayerSetTransProto() : FkIntVec2Proto() {

}

FkLayerSetTransProto::FkLayerSetTransProto(const FkLayerSetTransProto &o)
        : FkIntVec2Proto(o), layer(o.layer) {

}

FkLayerSetTransProto::~FkLayerSetTransProto() {

}