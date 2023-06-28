/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkSetSizeProto.h"

FK_IMPL_CLASS_TYPE(FkSetSizeProto, FkProtocol)

FkSetSizeProto::FkSetSizeProto() : FkProtocol(), value(0, 0) {

}

FkSetSizeProto::FkSetSizeProto(const FkSetSizeProto &o) : FkProtocol(o), value(o.value) {

}

FkSetSizeProto::~FkSetSizeProto() {
    value.set(0, 0);
}