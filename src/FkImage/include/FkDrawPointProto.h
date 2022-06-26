/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKDRAWPOINTPROTO_H
#define FK_GRAPHIC_FKDRAWPOINTPROTO_H

#include "FkProtocol.h"
#include "FkDoubleVec2.h"
#include "FkColor.h"

FK_SUPER_CLASS(FkDrawPointProto, FkProtocol) {
FK_DEF_CLASS_TYPE_FUNC(FkDrawPointProto)

public:
    FkDrawPointProto();

    FkDrawPointProto(const FkDrawPointProto &o);

    virtual ~FkDrawPointProto();

public:
    FkID layer;
    FkDoubleVec2 value;
    FkColor color;
    int32_t size = 0;
};

#endif //FK_GRAPHIC_FKDRAWPOINTPROTO_H