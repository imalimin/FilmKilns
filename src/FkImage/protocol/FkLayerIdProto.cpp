/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-4-16 21:06:28
*/

#include "FkLayerIdProto.h"

FK_IMPL_CLASS_TYPE(FkLayerIdProto, FkProtocol)

FkLayerIdProto::FkLayerIdProto(FkID layerId) : FkProtocol(), layerId(layerId) {

}

FkLayerIdProto::FkLayerIdProto(const FkLayerIdProto &o) : FkProtocol(o), layerId(o.layerId) {

}

FkLayerIdProto::~FkLayerIdProto() {
    layerId = FK_ID_NONE;
}