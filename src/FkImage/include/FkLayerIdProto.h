/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-4-16 21:06:28
*/

#ifndef FK_IMAGE_FKLAYERIDPROTO_H
#define FK_IMAGE_FKLAYERIDPROTO_H

#include "FkProtocol.h"

FK_CLASS FkLayerIdProto FK_EXTEND FkProtocol {
public:
    FkLayerIdProto(FkID layerId);

    FkLayerIdProto(const FkLayerIdProto &o);

    virtual ~FkLayerIdProto();

public:
    FkID layerId = FK_ID_NONE;
};

#endif //FK_IMAGE_FKLAYERIDPROTO_H