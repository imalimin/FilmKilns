/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-4-19 13:35:25
*/

#include "FkRect.h"

FkIntRect::FkIntRect(int32_t left, int32_t top, int32_t right, int32_t bottom)
        : FkRect<int32_t>(left, top, right, bottom) {
    FK_MARK_SUPER
}

FkIntRect::FkIntRect(const FkIntRect &o) : FkRect<int32_t>(o) {
    FK_MARK_SUPER
}

FkIntRect::~FkIntRect() {

}

FkFloatRect::FkFloatRect(float left, float top, float right, float bottom)
        : FkRect<float>(left, top, right, bottom) {
    FK_MARK_SUPER
}

FkFloatRect::FkFloatRect(const FkFloatRect &o) : FkRect<float>(o) {
    FK_MARK_SUPER
}

FkFloatRect::~FkFloatRect() {

}