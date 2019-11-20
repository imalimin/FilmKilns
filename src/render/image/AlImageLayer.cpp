/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlImageLayer.h"
#include "HwTexture.h"

AlImageLayer *AlImageLayer::create(HwAbsTexture *tex) {
    return new AlImageLayer(tex);
}

AlImageLayer::AlImageLayer(HwAbsTexture *tex) : Object() {
    this->tex = tex;
}

AlImageLayer::~AlImageLayer() {

}

void AlImageLayer::draw(AlImageCanvas *canvas) {

}

int32_t AlImageLayer::getWidth() {
    return this->tex->getWidth();
}

int32_t AlImageLayer::getHeight() {
    return this->tex->getHeight();
}
