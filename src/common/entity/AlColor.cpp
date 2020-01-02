/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlColor.h"

AlColor AlColor::from(std::string colorStr) {
    return AlColor(0);
}

AlColor::AlColor(int32_t color) : Object(), _color(color) {

}

AlColor::AlColor(const AlColor &o) : Object(), _color(o._color) {

}

AlColor::~AlColor() {

}

int32_t AlColor::color() {
    return _color;
}

uint8_t AlColor::r() {
    return static_cast<uint8_t>((_color >> 16) & 0xff);
}

uint8_t AlColor::g() {
    return static_cast<uint8_t>((_color >> 8) & 0xff);
}

uint8_t AlColor::b() {
    return static_cast<uint8_t>(_color & 0xff);
}

uint8_t AlColor::a() {
    return static_cast<uint8_t>((_color >> 24) & 0xff);
}

float AlColor::rf() {
    return r() / 255.0f;
}

float AlColor::gf() {
    return g() / 255.0f;
}

float AlColor::bf() {
    return b() / 255.0f;
}

float AlColor::af() {
    return a() / 255.0f;
}
