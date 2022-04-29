/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkColor.h"

FkColor &FkColor::makeFrom(int32_t color) {
    static FkColor *c;
    if (c == nullptr) {
        c = new FkColor();
    }
    c->alpha = (color >> 24) & 0x000000FF;
    c->red = (color >> 16) & 0x000000FF;
    c->green = (color >> 8) & 0x000000FF;
    c->blue = (color >> 0) & 0x000000FF;
    return *c;
}

FkColor &FkColor::makeFromRGBA8(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
    static FkColor *c;
    if (c == nullptr) {
        c = new FkColor();
    }
    c->alpha = alpha;
    c->red = red;
    c->green = green;
    c->blue = blue;
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

FkColor &FkColor::transparent() {
    static FkColor *color;
    if (color == nullptr) {
        color = new FkColor();
    }
    color->red = 0;
    color->green = 0;
    color->blue = 0;
    color->alpha = 0;
    return *color;
}

FkColor::FkColor() : FkObject(), alphaType(AlphaType::kUnPreMultiple) {
    FK_MARK_SUPER
}

FkColor::FkColor(const FkColor &o)
        : FkObject(), format(o.format), alphaType(o.alphaType),
          red(o.red), green(o.green), blue(o.blue), alpha(o.alpha) {
    FK_MARK_SUPER
}

FkColor::~FkColor() {
    delete[] _fArray;
}

float FkColor::fRed() {
    return red * (alphaType == AlphaType::kPreMultiple ? fAlpha() : 1.0f) / 255.0f;
}

float FkColor::fGreen() {
    return green * (alphaType == AlphaType::kPreMultiple ? fAlpha() : 1.0f) / 255.0f;
}

float FkColor::fBlue() {
    return blue * (alphaType == AlphaType::kPreMultiple ? fAlpha() : 1.0f) / 255.0f;
}

float FkColor::fAlpha() {
    return alpha / 255.0f;
}

uint16_t FkColor::uRed() {
    return red;
}

uint16_t FkColor::uGreen() {
    return green;
}

uint16_t FkColor::uBlue() {
    return blue;
}

uint16_t FkColor::uAlpha() {
    return alpha;
}

float *FkColor::fArray() {
    if (nullptr == _fArray) {
        _fArray = new float[4];
    }
    auto _alpha = fAlpha();
    _fArray[0] = fRed();
    _fArray[1] = fGreen();
    _fArray[2] = fBlue();
    _fArray[3] = _alpha;
    return _fArray;
}

int32_t FkColor::toInt() {
    int32_t color = 0;
    color |= ((uint8_t) alpha);
    color = color << 8;
    color |= ((uint8_t) (red * fAlpha()));
    color = color << 8;
    color |= ((uint8_t) (green * fAlpha()));
    color = color << 8;
    color |= ((uint8_t) (blue * fAlpha()));
    return color;
}

void FkColor::setAlphaType(AlphaType type) {
    this->alphaType = type;
}

FkColor::kFormat FkColor::getFormat() {
    return format;
}
