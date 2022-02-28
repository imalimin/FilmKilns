/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-28 22:22:29
*/

#include "FkWindowProto.h"

FkWindowProto::FkWindowProto() : FkProtocol() {
    FK_MARK_SUPER
}

FkWindowProto::FkWindowProto(const FkWindowProto &o) : FkProtocol(o), win(o.win) {
    FK_MARK_SUPER
}

FkWindowProto::~FkWindowProto() {

}