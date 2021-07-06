/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkSetSizeProto.h"

FkSetSizeProto::FkSetSizeProto() : FkProtocol(), value(0, 0) {
    FK_MARK_SUPER
}

FkSetSizeProto::FkSetSizeProto(const FkSetSizeProto &o) : FkProtocol(o), value(o.value) {
    FK_MARK_SUPER
}

FkSetSizeProto::~FkSetSizeProto() {
    value.set(0, 0);
}