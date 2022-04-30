/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-3-13 20:28:16
*/

#include "FkRationalProto.h"

FK_IMPL_CLASS_TYPE(FkRationalProto, FkProtocol)

FkRationalProto::FkRationalProto() : FkProtocol(), value(0, 1) {

}

FkRationalProto::FkRationalProto(const FkRationalProto &o) : FkProtocol(o), value(o.value) {

}

FkRationalProto::~FkRationalProto() {

}