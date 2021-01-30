/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkSize.h"

FkSize::FkSize() : FkObject(), width(0), height(0) {

}

FkSize::FkSize(int32_t width, int32_t height) : FkObject(), width(width), height(height) {

}

FkSize::FkSize(const FkSize &o) : FkObject(o), width(o.width), height(o.height) {

}

FkSize::~FkSize() {

}

void FkSize::set(int32_t width, int32_t height) {
    this->width = width;
    this->height = height;
}

int32_t FkSize::getWidth() {
    return width;
}

int32_t FkSize::getHeight() {
    return height;
}

bool FkSize::operator==(const FkSize &o) {
    return width == o.width && height == o.height;
}
