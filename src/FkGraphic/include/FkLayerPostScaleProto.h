/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKLAYERSCALEPROTO_H
#define FK_GRAPHIC_FKLAYERSCALEPROTO_H

#include "FkProtocol.h"
#include "FkFloatVec3.h"

FK_CLASS FkLayerPostScaleProto FK_EXTEND FkProtocol {
public:
    FkLayerPostScaleProto();

    FkLayerPostScaleProto(const FkLayerPostScaleProto &o);

    virtual ~FkLayerPostScaleProto();

public:
    FkID layer = FK_ID_NONE;
    FkFloatVec3 value;
};

#endif //FK_GRAPHIC_FKLAYERSCALEPROTO_H