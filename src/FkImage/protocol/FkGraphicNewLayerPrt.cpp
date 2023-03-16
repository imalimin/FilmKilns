/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicNewLayerPrt.h"

FK_IMPL_CLASS_TYPE(FkGraphicNewLayerPrt, FkLayerProto)

FkGraphicNewLayerPrt::FkGraphicNewLayerPrt() : FkLayerProto() {

}

FkGraphicNewLayerPrt::FkGraphicNewLayerPrt(const FkGraphicNewLayerPrt &o)
        : FkLayerProto(o), expectId(o.expectId) {

}

FkGraphicNewLayerPrt::~FkGraphicNewLayerPrt() {

}