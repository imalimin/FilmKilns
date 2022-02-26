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
    c->red = (color >> 24) & 0x000000FF;
    c->green = (color >> 16) & 0x000000FF;
    c->blue = (color >> 8) & 0x000000FF;
    c->alpha = (color >> 0) & 0x000000FF;
    return *c;
}

FkColor &FkColor::white() {
    static FkColor *color;
    if (color == nullptr) {
        color = new FkColor();
    }
    color->red = 255;
    color->green = 255;
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
    color->green = 0;
    color->blue = 0;
    color->alpha = 255;
    return *color;
}

FkColor::FkColor() {
    FK_MARK_SUPER
}

FkColor::FkColor(const FkColor &o)
        : FkObject(), format(o.format), red(o.red), green(o.green), blue(o.blue), alpha(o.alpha) {
    FK_MARK_SUPER
}

FkColor::~FkColor() {
    delete[] _fArray;
}

float FkColor::fRed() {
    return red / 255.0f;
}

float FkColor::fGreen() {
    return green / 255.0f;
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
    int32_t color = 0;
    color |= ((uint8_t) alpha);
    color = color << 8;
    color |= ((uint8_t) red);
    color = color << 8;
    color |= ((uint8_t) green);
    color = color << 8;
    color |= ((uint8_t) blue);
    return color;
}
