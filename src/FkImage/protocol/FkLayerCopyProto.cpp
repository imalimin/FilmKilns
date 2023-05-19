/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2023-5-19 0:10:49
*/

#include "FkLayerCopyProto.h"

FK_IMPL_CLASS_TYPE(FkLayerCopyProto, FkProtocol)

FkLayerCopyProto::FkLayerCopyProto(FkID src, FkID dst)
        : FkProtocol(), srcLayerId(src), dstLayerId(dst) {

}

FkLayerCopyProto::FkLayerCopyProto(const FkLayerCopyProto &o)
        : FkProtocol(o), srcLayerId(o.srcLayerId), dstLayerId(o.dstLayerId) {

}

FkLayerCopyProto::~FkLayerCopyProto() {
}
