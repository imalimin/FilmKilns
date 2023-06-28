/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-4-16 21:06:37
*/

#include "FkRemoveLayerProto.h"

FK_IMPL_CLASS_TYPE(FkRemoveLayerProto, FkLayerIdProto)

FkRemoveLayerProto::FkRemoveLayerProto() : FkLayerIdProto(FK_ID_NONE) {

}

FkRemoveLayerProto::FkRemoveLayerProto(FkID layerId) : FkLayerIdProto(layerId) {

}

FkRemoveLayerProto::FkRemoveLayerProto(const FkRemoveLayerProto &o) : FkLayerIdProto(o) {

}

FkRemoveLayerProto::~FkRemoveLayerProto() {

}