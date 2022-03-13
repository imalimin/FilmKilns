/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicNewLayerPrt.h"

FkGraphicNewLayerPrt::FkGraphicNewLayerPrt() : FkProtocol() {
    FK_MARK_SUPER
}

FkGraphicNewLayerPrt::FkGraphicNewLayerPrt(const FkGraphicNewLayerPrt &o)
        : FkProtocol(o), layer(o.layer), expectId(o.expectId) {
    FK_MARK_SUPER
    if (o.layer) {
        this->layer = std::make_shared<FkGraphicLayer>(*o.layer);
    }
}

FkGraphicNewLayerPrt::~FkGraphicNewLayerPrt() {

}