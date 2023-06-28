/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-3-13 13:22:55
*/

#include "FkLayerSetScaleProto.h"

FK_IMPL_CLASS_TYPE(FkLayerSetScaleProto, FkFloatVec3Proto)

FkLayerSetScaleProto::FkLayerSetScaleProto()
        : FkFloatVec3Proto(), layer(FK_ID_NONE) {

}

FkLayerSetScaleProto::FkLayerSetScaleProto(const FkLayerSetScaleProto &o)
        : FkFloatVec3Proto(o), layer(o.layer) {

}

FkLayerSetScaleProto::~FkLayerSetScaleProto() {

}