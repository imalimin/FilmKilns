/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicUpdateLayerPrt.h"

FkGraphicUpdateLayerPrt::FkGraphicUpdateLayerPrt() : FkProtocol() {
    FK_MARK_SUPER
}

FkGraphicUpdateLayerPrt::FkGraphicUpdateLayerPrt(const FkGraphicUpdateLayerPrt &o) : FkProtocol(o), layer(o.layer) {
    FK_MARK_SUPER
    if (o.layer) {
        this->layer = std::make_shared<FkGraphicLayer>(*(o.layer.get()));
    }
}

FkGraphicUpdateLayerPrt::~FkGraphicUpdateLayerPrt() {

}