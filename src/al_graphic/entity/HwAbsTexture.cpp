/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "../include/HwAbsTexture.h"

HwAbsTexture::HwAbsTexture(AlTexDescription desc) : Object(),
                                                    desc(desc) {

}

HwAbsTexture::~HwAbsTexture() {
    desc.target = GL_NONE;
    desc.size.width = 0;
    desc.size.height = 0;
}

uint32_t HwAbsTexture::texId() {
    return tex;
}

int HwAbsTexture::getWidth() {
    return desc.size.width;
}

int HwAbsTexture::getHeight() {
    return desc.size.height;
}

uint32_t HwAbsTexture::target() {
    return desc.target;
}

void HwAbsTexture::update(AlBuffer *buf, int32_t w, int32_t h, uint32_t fmt) {
    desc.size.width = w;
    desc.size.height = h;
    desc.fmt = fmt;
}

uint32_t HwAbsTexture::fmt() { return desc.fmt; }

size_t HwAbsTexture::countOfByte() {
    int countOfPlane = 1;
    switch (desc.fmt) {
        case GL_RGBA: {
            countOfPlane = 4;
            break;
        }
        case GL_RGB: {
            countOfPlane = 3;
            break;
        }
        case GL_RGBA4:
        case GL_RGB565: {
            countOfPlane = 2;
            break;
        }
        case GL_ALPHA:
        default:
            countOfPlane = 1;
    }
    return static_cast<size_t>(desc.size.width * desc.size.height * countOfPlane);
}

HwAbsTexture::HwAbsTexture(const HwAbsTexture &o) : Object(), desc(o.desc), tex(o.tex) {

}

AlTexDescription::AlTexDescription()
        : AlTexDescription(GL_TEXTURE_2D, GL_RGBA) {
}

AlTexDescription::AlTexDescription(uint32_t target, uint32_t fmt)
        : Object(),
          target(target),
          wrapMode(AlTexDescription::WrapMode::REPEAT),
          fmt(fmt) {
    borderColor[0] = 1.0f;
    borderColor[1] = 1.0f;
    borderColor[2] = 1.0f;
    borderColor[3] = 1.0f;
    size.width = 0;
    size.height = 0;
}

AlTexDescription::~AlTexDescription() {

}

AlTexDescription::AlTexDescription(const AlTexDescription &o)
        : Object(),
          target(o.target),
          wrapMode(o.wrapMode),
          size(o.size),
          fmt(o.fmt) {
    borderColor[0] = o.borderColor[0];
    borderColor[1] = o.borderColor[1];
    borderColor[2] = o.borderColor[2];
    borderColor[3] = o.borderColor[3];

}
