/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKLAYERROTATEPROTO_H
#define FK_GRAPHIC_FKLAYERROTATEPROTO_H

#include "FkProtocol.h"
#include "FkFloatVec3.h"

FK_CLASS FkLayerPostRotateProto FK_EXTEND FkProtocol {
public:
    FkLayerPostRotateProto();

    FkLayerPostRotateProto(const FkLayerPostRotateProto &o);

    virtual ~FkLayerPostRotateProto();

public:
    FkID layer = FK_ID_NONE;
    FkFloatVec3 value;
};

#endif //FK_GRAPHIC_FKLAYERROTATEPROTO_H