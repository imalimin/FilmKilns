/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkMeasureTransProto.h"

FK_IMPL_CLASS_TYPE(FkMeasureTransProto, FkWinSizeProto)

FkMeasureTransProto::FkMeasureTransProto() : FkWinSizeProto(), value(0, 0) {

}

FkMeasureTransProto::FkMeasureTransProto(const FkMeasureTransProto &o)
        : FkWinSizeProto(o), layerId(o.layerId), value(o.value), layer(o.layer) , canvas(o.canvas) {

}

FkMeasureTransProto::~FkMeasureTransProto() {

}