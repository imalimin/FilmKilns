/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkColor.h"

FK_IMPL_CLASS_TYPE(FkColor, FkObject)

FkColor FkColor::makeFrom(int32_t color) {
    return makeFromRGBA8((color >> 16) & 0x000000FF, (color >> 8) & 0x000000FF,
                         (color >> 0) & 0x000000FF, (color >> 24) & 0x000000FF);
}

FkColor FkColor::makeFromRGBA8(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
    FkColor color;
    color.vec[0] = red;
    color.vec[1] = green;
    color.vec[2] = blue;
    color.vec[3] = alpha;
    return color;
}

FkColor FkColor::white() {
    return makeFromRGBA8(255, 255, 255, 255);
}

FkColor FkColor::black() {
    return makeFromRGBA8(0, 0, 0, 255);
}

FkColor FkColor::red() {
    return makeFromRGBA8(255, 0, 0, 255);
}

FkColor FkColor::green() {
    return makeFromRGBA8(0, 255, 0, 255);
}

FkColor FkColor::blue() {
    return makeFromRGBA8(0, 0, 255, 255);
}

FkColor FkColor::transparent() {
    return makeFromRGBA8(0, 0, 0, 0);
}

FkColor::FkColor() : FkObject(), alphaType(AlphaType::kUnPreMultiple) {
    vec.resize(4);
}

FkColor::FkColor(const FkColor &o)
        : FkObject(), format(o.format), alphaType(o.alphaType) {
    vec.resize(4);
    memcpy(vec.data(), o.vec.data(), sizeof(uint16_t) * vec.size());
}

FkColor::~FkColor() {
    if (_fArray) {
        delete[] _fArray;
    }
}

float FkColor::fRed() {
    return uRed() * (alphaType == AlphaType::kPreMultiple ? fAlpha() : 1.0f) / 255.0f;
}

float FkColor::fGreen() {
    return uGreen() * (alphaType == AlphaType::kPreMultiple ? fAlpha() : 1.0f) / 255.0f;
}

float FkColor::fBlue() {
    return uBlue() * (alphaType == AlphaType::kPreMultiple ? fAlpha() : 1.0f) / 255.0f;
}

float FkColor::fAlpha() {
    return uAlpha() / 255.0f;
}

uint16_t FkColor::uRed() {
    return vec[0];
}

uint16_t FkColor::uGreen() {
    return vec[1];
}

uint16_t FkColor::uBlue() {
    return vec[2];
}

uint16_t FkColor::uAlpha() {
    return vec[3];
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
    color |= ((uint8_t) uAlpha());
    color = color << 8;
    color |= ((uint8_t) (uRed() * (isPreMultiple() ? fAlpha() : 1)));
    color = color << 8;
    color |= ((uint8_t) (uGreen() * (isPreMultiple() ? fAlpha() : 1)));
    color = color << 8;
    color |= ((uint8_t) (uBlue() * (isPreMultiple() ? fAlpha() : 1)));
    return color;
}

void FkColor::setAlphaType(AlphaType type) {
    this->alphaType = type;
}

bool FkColor::isPreMultiple() {
    return alphaType == AlphaType::kPreMultiple;
}

FkColor::kFormat FkColor::getFormat() {
    return format;
}

bool FkColor::equals(FkColor &color) {
    return uRed() == color.uRed() && uGreen() == color.uGreen() && uBlue() == color.uBlue() && uAlpha() == color.uAlpha();
}
