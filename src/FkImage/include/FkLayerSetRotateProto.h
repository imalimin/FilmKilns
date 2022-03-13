/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-3-13 13:23:03
*/

#ifndef FK_IMAGE_FKLAYERSETROTATEPROTO_H
#define FK_IMAGE_FKLAYERSETROTATEPROTO_H

#include "FkRationalProto.h"

FK_CLASS FkLayerSetRotateProto FK_EXTEND FkRationalProto {
public:
    FkLayerSetRotateProto();

    FkLayerSetRotateProto(const FkLayerSetRotateProto &o);

    virtual ~FkLayerSetRotateProto();

public:
    FkID layer = FK_ID_NONE;
};

#endif //FK_IMAGE_FKLAYERSETROTATEPROTO_H