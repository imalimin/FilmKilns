/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkFloatVec2.h"

FkFloatVec2::FkFloatVec2(float x, float y) : FkVec2<float>(x, y) {
    FK_MARK_SUPER
}

FkFloatVec2::FkFloatVec2(const FkFloatVec2 &o) : FkVec2<float>(o) {
    FK_MARK_SUPER
}

FkFloatVec2::~FkFloatVec2() {

}

bool FkFloatVec2::isNan() {
    return isnanf(x) || isnanf(y);
}