/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-4-19 23:37:02
*/

#include "FkLayerProto.h"

FkLayerProto::FkLayerProto() : FkProtocol() {
    FK_MARK_SUPER
}

FkLayerProto::FkLayerProto(const FkLayerProto &o)
        : FkProtocol(o), layerId(o.layerId) {
    FK_MARK_SUPER
    if (o.layer) {
        this->layer = std::make_shared<FkGraphicLayer>(*o.layer);
    }
}

FkLayerProto::~FkLayerProto() {

}