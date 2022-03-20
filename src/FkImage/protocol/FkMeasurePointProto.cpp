/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-3-20 11:21:49
*/

#include "FkMeasurePointProto.h"

FkMeasurePointProto::FkMeasurePointProto() : FkMeasureTransProto() {
    FK_MARK_SUPER
}

FkMeasurePointProto::FkMeasurePointProto(const FkMeasurePointProto &o) : FkMeasureTransProto(o) {
    FK_MARK_SUPER
}

FkMeasurePointProto::~FkMeasurePointProto() {

}