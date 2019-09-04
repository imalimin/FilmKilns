/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "../include/HwAbsTexture.h"

HwAbsTexture::HwAbsTexture(uint32_t target) : Object(),
                                              tar(target) {

}

HwAbsTexture::~HwAbsTexture() {
    size.width = 0;
    size.height = 0;
}

uint32_t HwAbsTexture::texId() {
    return tex;
}

int HwAbsTexture::getWidth() {
    return size.width;
}

int HwAbsTexture::getHeight() {
    return size.height;
}

uint32_t HwAbsTexture::target() {
    return tar;
}

void HwAbsTexture::update(HwBuffer *buf, int32_t w, int32_t h, uint32_t fmt) {
    size.width = w;
    size.height = h;
    _fmt = fmt;
}

uint32_t HwAbsTexture::fmt() { return _fmt; }