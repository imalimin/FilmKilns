/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkColor.h"

FkColor &FkColor::from(int32_t color) {
    static FkColor *c;
    if (c == nullptr) {
        c = new FkColor();
    }
    c->red = color & 0x00FF0000;
    c->greed = color & 0x0000FF00;
    c->blue = color & 0x000000FF;
    c->alpha = color & 0xFF000000;
    return *c;
}

FkColor &FkColor::white() {
    static FkColor *color;
    if (color == nullptr) {
        color = new FkColor();
    }
    color->red = 255;
    color->greed = 255;
    color->blue = 255;
    color->alpha = 255;
    return *color;
}

FkColor &FkColor::black() {
    static FkColor *color;
    if (color == nullptr) {
        color = new FkColor();
    }
    color->red = 0;
    color->greed = 0;
    color->blue = 0;
    color->alpha = 255;
    return *color;
}

FkColor::FkColor() {
    FK_MARK_SUPER
}

FkColor::FkColor(const FkColor &o)
        : FkObject(), format(o.format), red(o.red), greed(o.greed), blue(o.blue), alpha(o.alpha) {
    FK_MARK_SUPER
}

FkColor::~FkColor() {
    delete[] _fArray;
}

float FkColor::fRed() {
    return red / 255.0f;
}

float FkColor::fGreen() {
    return greed / 255.0f;
}

float FkColor::fBlue() {
    return blue / 255.0f;
}

float FkColor::fAlpha() {
    return alpha / 255.0f;
}

float *FkColor::fArray() {
    if (nullptr == _fArray) {
        _fArray = new float[4];
    }
    _fArray[0] = fRed();
    _fArray[1] = fGreen();
    _fArray[2] = fBlue();
    _fArray[3] = fAlpha();
    return _fArray;
}

int32_t FkColor::toInt() {
    int32_t color = 0xFF000000;
    color |= (((uint8_t) red) << 16);
    color |= (((uint8_t) greed) << 8);
    color |= (((uint8_t) blue) << 0);
    return color;
}
