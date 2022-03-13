/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-3-13 13:21:51
*/

#ifndef FK_IMAGE_FKLAYERSETTRANSPROTO_H
#define FK_IMAGE_FKLAYERSETTRANSPROTO_H

#include "FkIntVec2Proto.h"

FK_CLASS FkLayerSetTransProto FK_EXTEND FkIntVec2Proto {
public:
    FkLayerSetTransProto();

    FkLayerSetTransProto(const FkLayerSetTransProto &o);

    virtual ~FkLayerSetTransProto();

public:
    FkID layer = FK_ID_NONE;
};

#endif //FK_IMAGE_FKLAYERSETTRANSPROTO_H