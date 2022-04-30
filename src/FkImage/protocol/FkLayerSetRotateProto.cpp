/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-3-13 13:23:03
*/

#include "FkLayerSetRotateProto.h"

FK_IMPL_CLASS_TYPE(FkLayerSetRotateProto, FkRationalProto)

FkLayerSetRotateProto::FkLayerSetRotateProto()
        : FkRationalProto(), layer(FK_ID_NONE) {

}

FkLayerSetRotateProto::FkLayerSetRotateProto(const FkLayerSetRotateProto &o)
        : FkRationalProto(o), layer(o.layer) {

}

FkLayerSetRotateProto::~FkLayerSetRotateProto() {

}