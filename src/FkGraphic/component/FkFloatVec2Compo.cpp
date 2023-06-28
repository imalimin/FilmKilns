/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-4-27 0:32:56
*/

#include "FkFloatVec2Compo.h"

FK_IMPL_CLASS_TYPE(FkFloatVec2Compo, FkComponent)

FkFloatVec2Compo::FkFloatVec2Compo(float x, float y) : FkComponent(), vec(x, y) {

}

FkFloatVec2Compo::FkFloatVec2Compo(const FkFloatVec2Compo &o) : FkComponent(o), vec(o.vec) {

}

FkFloatVec2Compo::~FkFloatVec2Compo() {

}