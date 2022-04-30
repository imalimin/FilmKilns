/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-3-13 13:22:55
*/

#ifndef FK_IMAGE_FKLAYERSETSCALEPROTO_H
#define FK_IMAGE_FKLAYERSETSCALEPROTO_H

#include "FkFloatVec3Proto.h"

FK_SUPER_CLASS(FkLayerSetScaleProto, FkFloatVec3Proto) {
FK_DEF_CLASS_TYPE_FUNC(FkLayerSetScaleProto)

public:
    FkLayerSetScaleProto();

    FkLayerSetScaleProto(const FkLayerSetScaleProto &o);

    virtual ~FkLayerSetScaleProto();

public:
    FkID layer = FK_ID_NONE;
};

#endif //FK_IMAGE_FKLAYERSETSCALEPROTO_H