/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "../include/Size.h"

Size::Size() : Object() {
    this->width = 0;
    this->height = 0;
};

Size::Size(const Size &value) : Object(), width(value.width), height(value.height) {
}

Size::Size(int w, int h) {
    this->width = w;
    this->height = h;
};

Size::~Size() {
    this->width = 0;
    this->height = 0;
};

Size &Size::operator=(const Size &value) {
    this->width = value.width;
    this->height = value.height;
    return *this;
}
