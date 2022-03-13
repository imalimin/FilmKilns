/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkLayerPostTransProto.h"

FkLayerPostTransProto::FkLayerPostTransProto() : FkIntVec2Proto(),
                                                 layer(FK_ID_NONE) {
    FK_MARK_SUPER
}

FkLayerPostTransProto::FkLayerPostTransProto(const FkLayerPostTransProto &o) : FkIntVec2Proto(o),
                                                                               layer(o.layer) {
    FK_MARK_SUPER
}

FkLayerPostTransProto::~FkLayerPostTransProto() {

}