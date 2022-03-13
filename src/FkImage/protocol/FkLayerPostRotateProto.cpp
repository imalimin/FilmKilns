/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkLayerPostRotateProto.h"

FkLayerPostRotateProto::FkLayerPostRotateProto()
        : FkRationalProto(), layer(FK_ID_NONE) {
    FK_MARK_SUPER
}

FkLayerPostRotateProto::FkLayerPostRotateProto(const FkLayerPostRotateProto &o)
        : FkRationalProto(o), layer(o.layer) {
    FK_MARK_SUPER
}

FkLayerPostRotateProto::~FkLayerPostRotateProto() {

}