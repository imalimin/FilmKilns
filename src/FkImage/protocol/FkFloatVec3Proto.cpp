/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-3-13 20:28:34
*/

#include "FkFloatVec3Proto.h"

FkFloatVec3Proto::FkFloatVec3Proto()
        : FkProtocol(), value(0.0f, 0.0f, 0.0f) {
    FK_MARK_SUPER
}

FkFloatVec3Proto::FkFloatVec3Proto(const FkFloatVec3Proto &o)
        : FkProtocol(o), value(o.value) {
    FK_MARK_SUPER
}

FkFloatVec3Proto::~FkFloatVec3Proto() {

}