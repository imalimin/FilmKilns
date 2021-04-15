/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkIntVec2.h"

FkIntVec2::FkIntVec2(int32_t x, int32_t y) : FkVec2<int32_t>(x, y) {
    FK_MARK_SUPER
}

FkIntVec2::FkIntVec2(const FkIntVec2 &o) : FkVec2<int32_t>(o) {
    FK_MARK_SUPER
}

FkIntVec2::~FkIntVec2() {

}