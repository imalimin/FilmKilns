/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkDrawPointProto.h"

FkDrawPointProto::FkDrawPointProto()
        : FkProtocol(), layer(FK_ID_NONE), value(0, 0), color(FkColor::black()) {
    FK_MARK_SUPER
}

FkDrawPointProto::FkDrawPointProto(const FkDrawPointProto &o)
        : FkProtocol(o), layer(o.layer), value(o.value), color(o.color), size(o.size) {
    FK_MARK_SUPER
}

FkDrawPointProto::~FkDrawPointProto() {

}