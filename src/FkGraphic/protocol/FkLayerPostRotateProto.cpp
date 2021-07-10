/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkLayerPostRotateProto.h"

FkLayerPostRotateProto::FkLayerPostRotateProto() : FkProtocol(),
                                                   layer(FK_ID_NONE),
                                                   value(0.0f, 0.0f, 0.0f) {
    FK_MARK_SUPER
}

FkLayerPostRotateProto::FkLayerPostRotateProto(const FkLayerPostRotateProto &o) : FkProtocol(o),
                                                                                  layer(o.layer),
                                                                                  value(o.value) {
    FK_MARK_SUPER
}

FkLayerPostRotateProto::~FkLayerPostRotateProto() {

}