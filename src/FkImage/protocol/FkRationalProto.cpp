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

FkRationalProto::FkRationalProto() : FkProtocol(), value(0, 1) {
    FK_MARK_SUPER
}

FkRationalProto::FkRationalProto(const FkRationalProto &o) : FkProtocol(o), value(o.value) {
    FK_MARK_SUPER
}

FkRationalProto::~FkRationalProto() {

}