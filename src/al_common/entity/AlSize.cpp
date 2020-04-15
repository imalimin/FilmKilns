/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlSize.h"

AlSize::AlSize() : Size() {

}

AlSize::AlSize(const AlSize &o) : Size(o) {

}

AlSize::AlSize(int w, int h) : Size(w, h) {

}

AlSize::~AlSize() {

}

AlSize &AlSize::operator=(const AlSize &o) {
    this->width = o.width;
    this->height = o.height;
    return *this;
}

bool AlSize::operator==(const AlSize &o) {
    bool v0 = this->width == o.width;
    bool v1 = this->height == o.height;
    return v0 && v1;
}

bool AlSize::operator!=(const AlSize &o) {
    bool v = *this == o;
    return !v;
}
