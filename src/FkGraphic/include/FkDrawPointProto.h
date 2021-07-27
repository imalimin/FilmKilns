/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKDRAWPOINTPROTO_H
#define FK_GRAPHIC_FKDRAWPOINTPROTO_H

#include "FkProtocol.h"
#include "FkIntVec2.h"
#include "FkColor.h"

FK_CLASS FkDrawPointProto FK_EXTEND FkProtocol {
public:
    FkDrawPointProto();

    FkDrawPointProto(const FkDrawPointProto &o);

    virtual ~FkDrawPointProto();

public:
    FkID layer;
    FkIntVec2 value;
    FkColor color;
};

#endif //FK_GRAPHIC_FKDRAWPOINTPROTO_H