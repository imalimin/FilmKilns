/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicLayerPrt.h"

FkGraphicLayerPrt::FkGraphicLayerPrt() : FkProtocol() {
    FK_MARK_SUPER
}

FkGraphicLayerPrt::FkGraphicLayerPrt(const FkGraphicLayerPrt &o) : FkProtocol(o) {
    FK_MARK_SUPER
    if (o.layer) {
        this->layer = std::make_shared<FkGraphicLayer>(*(o.layer.get()));
    }
}

FkGraphicLayerPrt::~FkGraphicLayerPrt() {

}
