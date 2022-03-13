/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKLAYERSCALEPROTO_H
#define FK_GRAPHIC_FKLAYERSCALEPROTO_H

#include "FkFloatVec3Proto.h"

FK_CLASS FkLayerPostScaleProto FK_EXTEND FkFloatVec3Proto {
public:
    FkLayerPostScaleProto();

    FkLayerPostScaleProto(const FkLayerPostScaleProto &o);

    virtual ~FkLayerPostScaleProto();

public:
    FkID layer = FK_ID_NONE;
};

#endif //FK_GRAPHIC_FKLAYERSCALEPROTO_H