/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKCALCTRANSPROTO_H
#define FK_GRAPHIC_FKCALCTRANSPROTO_H

#include "FkWinSizeProto.h"
#include "FkIntVec2.h"
#include "FkGraphicLayer.h"

FK_SUPER_CLASS(FkMeasureTransProto, FkWinSizeProto) {
FK_DEF_CLASS_TYPE_FUNC(FkMeasureTransProto)

public:
    FkMeasureTransProto();

    FkMeasureTransProto(const FkMeasureTransProto &o);

    virtual ~FkMeasureTransProto();

public:
    FkID layerId = FK_ID_NONE;
    FkIntVec2 value;
    std::shared_ptr<FkGraphicLayer> layer = nullptr;
    std::shared_ptr<FkGraphicLayer> canvas = nullptr;
};

#endif //FK_GRAPHIC_FKCALCTRANSPROTO_H