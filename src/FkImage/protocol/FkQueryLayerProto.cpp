/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2022-07-31 10:33:21
*/

#include "FkQueryLayerProto.h"

FK_IMPL_CLASS_TYPE(FkQueryLayerProto, FkProtocol)

FkQueryLayerProto::FkQueryLayerProto(FkID layerId) : FkProtocol(), layerId(layerId) {

}

FkQueryLayerProto::FkQueryLayerProto(const FkQueryLayerProto &o)
        : FkProtocol(o), layerId(o.layerId), layer(o.layer) {

}

FkQueryLayerProto::~FkQueryLayerProto() {

}