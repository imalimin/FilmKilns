/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicUpdateLayerPrt.h"

FK_IMPL_CLASS_TYPE(FkGraphicUpdateLayerPrt, FkWinSizeProto)

FkGraphicUpdateLayerPrt::FkGraphicUpdateLayerPrt()
        : FkWinSizeProto(), scaleType(kScaleType::CENTER_INSIDE) {

}

FkGraphicUpdateLayerPrt::FkGraphicUpdateLayerPrt(const FkGraphicUpdateLayerPrt &o)
        : FkWinSizeProto(o), layer(o.layer), scaleType(o.scaleType) {

    if (o.layer) {
        this->layer = std::make_shared<FkGraphicLayer>(*(o.layer.get()));
    }
}

FkGraphicUpdateLayerPrt::~FkGraphicUpdateLayerPrt() {

}