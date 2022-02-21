/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-22 0:11:10
*/

#include "FkGenIDProto.h"

FkGenIDProto::FkGenIDProto() : FkProtocol() {
    FK_MARK_SUPER
}

FkGenIDProto::FkGenIDProto(const FkGenIDProto &o) : FkProtocol(o) {
    FK_MARK_SUPER
}

FkGenIDProto::~FkGenIDProto() {

}