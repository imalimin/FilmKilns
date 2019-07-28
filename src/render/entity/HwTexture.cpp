/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "../include/HwTexture.h"

HwTexture::HwTexture(uint32_t tex, int w, int h) : Object(), tex(tex), size(Size(w, h)) {

}

HwTexture::~HwTexture() {
    tex = 0;
}

uint32_t HwTexture::texId() {
    return tex;
}

int HwTexture::getWidth() {
    return size.width;
}

int HwTexture::getHeight() {
    return size.height;
}