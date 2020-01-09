/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlImageLayer.h"
#include "HwTexture.h"
#include "AlVec4.h"
#include "AlMath.h"
#include "Logcat.h"

#define TAG "AlImageLayer"

AlImageLayer *AlImageLayer::create(HwAbsTexture *tex) {
    return new AlImageLayer(tex);
}

AlImageLayer::AlImageLayer(HwAbsTexture *tex) : Object() {
    this->tex = tex;
}

AlImageLayer::~AlImageLayer() {
}

int32_t AlImageLayer::getWidth() {
    return this->tex->getWidth();
}

int32_t AlImageLayer::getHeight() {
    return this->tex->getHeight();
}

HwAbsTexture *AlImageLayer::getTexture() {
    return tex;
}
