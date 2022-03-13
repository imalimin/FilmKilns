/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkLayerPostScaleProto.h"

FkLayerPostScaleProto::FkLayerPostScaleProto()
        : FkFloatVec3Proto(), layer(FK_ID_NONE) {
    FK_MARK_SUPER
}

FkLayerPostScaleProto::FkLayerPostScaleProto(const FkLayerPostScaleProto &o)
        : FkFloatVec3Proto(o), layer(o.layer) {
    FK_MARK_SUPER
}

FkLayerPostScaleProto::~FkLayerPostScaleProto() {

}