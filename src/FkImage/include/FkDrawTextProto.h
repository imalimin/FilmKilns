/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2023-08-31 12:46:16
*/

#ifndef FK_IMAGE_FKDRAWTEXTPROTO_H
#define FK_IMAGE_FKDRAWTEXTPROTO_H

#include "FkProtocol.h"
#include "FkIntVec2.h"
#include "FkPaint.h"

FK_SUPER_CLASS(FkDrawTextProto, FkProtocol) {
FK_DEF_CLASS_TYPE_FUNC(FkDrawTextProto)

public:
    FkDrawTextProto(FkID layerId, bool isClear);

    FkDrawTextProto(FkID layerId, const std::string &text, const FkIntVec2 &pos, std::shared_ptr<FkPaint> &paint);

    FkDrawTextProto(const FkDrawTextProto &o);

    virtual ~FkDrawTextProto();

public:
    FkID layerId;
    std::string text;
    FkIntVec2 pos;
    std::shared_ptr<FkPaint> paint;
    bool isClear = false;
};

#endif //FK_IMAGE_FKDRAWTEXTPROTO_H