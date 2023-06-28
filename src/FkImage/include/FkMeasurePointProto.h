/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-3-20 11:21:49
*/

#ifndef FK_IMAGE_FKMEASUREPOINTPROTO_H
#define FK_IMAGE_FKMEASUREPOINTPROTO_H

#include "FkMeasureTransProto.h"

FK_SUPER_CLASS(FkMeasurePointProto, FkMeasureTransProto) {
FK_DEF_CLASS_TYPE_FUNC(FkMeasurePointProto)

public:
    FkMeasurePointProto();

    FkMeasurePointProto(const FkMeasurePointProto &o);

    virtual ~FkMeasurePointProto();
};

#endif //FK_IMAGE_FKMEASUREPOINTPROTO_H