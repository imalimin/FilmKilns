/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkFloatVec3.h"

FK_IMPL_CLASS_TYPE(FkFloatVec3, FkVec3)

FkFloatVec3::FkFloatVec3(float x, float y, float z) : FkVec3<float>(x, y, z) {

}

FkFloatVec3::FkFloatVec3(const FkFloatVec3 &o) : FkVec3<float>(o) {

}

FkFloatVec3::~FkFloatVec3() {

}