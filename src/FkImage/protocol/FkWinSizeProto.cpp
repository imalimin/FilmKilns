/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkWinSizeProto.h"

FkWinSizeProto::FkWinSizeProto() : FkProtocol(), winSize(0, 0) {
    FK_MARK_SUPER
}

FkWinSizeProto::FkWinSizeProto(const FkWinSizeProto &o) : FkProtocol(o), winSize(o.winSize) {
    FK_MARK_SUPER
}

FkWinSizeProto::~FkWinSizeProto() {

}