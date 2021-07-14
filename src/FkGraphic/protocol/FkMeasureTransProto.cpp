/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkMeasureTransProto.h"

FkMeasureTransProto::FkMeasureTransProto() : FkProtocol(), value(0, 0) {
    FK_MARK_SUPER
}

FkMeasureTransProto::FkMeasureTransProto(const FkMeasureTransProto &o)
        : FkProtocol(o), layerId(o.layerId), value(o.value), layer(o.layer) , canvas(o.canvas) {
    FK_MARK_SUPER
}

FkMeasureTransProto::~FkMeasureTransProto() {

}