/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2023-08-31 12:46:16
*/

#include "FkDrawTextProto.h"

FK_IMPL_CLASS_TYPE(FkDrawTextProto, FkProtocol)

FkDrawTextProto::FkDrawTextProto(FkID layerId, bool isClear)
        : FkProtocol(), layerId(layerId), text(""), pos(0, 0), paint(), isClear(isClear) {

}

FkDrawTextProto::FkDrawTextProto(FkID layerId,
                                 const std::string &text,
                                 const FkIntVec2 &pos,
                                 std::shared_ptr<FkPaint> &paint)
        : FkProtocol(), layerId(layerId), text(text), pos(pos), paint(paint) {

}

FkDrawTextProto::FkDrawTextProto(const FkDrawTextProto &o)
        : FkProtocol(o), layerId(o.layerId), text(o.text), pos(o.pos), paint(o.paint) {

}

FkDrawTextProto::~FkDrawTextProto() {

}