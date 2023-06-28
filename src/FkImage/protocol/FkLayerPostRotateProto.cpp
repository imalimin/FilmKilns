/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkLayerPostRotateProto.h"

FK_IMPL_CLASS_TYPE(FkLayerPostRotateProto, FkRationalProto)

FkLayerPostRotateProto::FkLayerPostRotateProto()
        : FkRationalProto(), layer(FK_ID_NONE) {

}

FkLayerPostRotateProto::FkLayerPostRotateProto(const FkLayerPostRotateProto &o)
        : FkRationalProto(o), layer(o.layer) {

}

FkLayerPostRotateProto::~FkLayerPostRotateProto() {

}