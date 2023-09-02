/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2023-09-02 21:32:22
*/

#include "FkSetZIndexProto.h"

FK_IMPL_CLASS_TYPE(FkSetZIndexProto, FkProtocol)

FkSetZIndexProto::FkSetZIndexProto(FkID layerId, int32_t zIndex)
        : FkProtocol(), layerId(layerId), zIndex(zIndex) {

}

FkSetZIndexProto::FkSetZIndexProto(const FkSetZIndexProto &o)
        : FkProtocol(o), layerId(o.layerId), zIndex(o.zIndex) {

}

FkSetZIndexProto::~FkSetZIndexProto() {

}