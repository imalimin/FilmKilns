/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkColor.h"

FkColor &FkColor::white() {
    static FkColor *color;
    if (color == nullptr) {
        color = new FkColor();
    }
    color->red = 0;
    color->greed = 0;
    color->blue = 0;
    color->alpha = 0;
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
