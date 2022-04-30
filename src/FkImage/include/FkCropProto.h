/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-3-27 14:57:36
*/

#ifndef FK_IMAGE_FKCROPPROTO_H
#define FK_IMAGE_FKCROPPROTO_H

#include "FkProtocol.h"
#include "FkIntVec2.h"
#include "FkGraphicLayer.h"

FK_SUPER_CLASS(FkCropProto, FkProtocol) {
FK_DEF_CLASS_TYPE_FUNC(FkCropProto)

public:
    FkCropProto();

    FkCropProto(const FkCropProto &o);

    virtual ~FkCropProto();

public:
    FkID layerId;
    std::shared_ptr<FkGraphicLayer> canvas = nullptr;
    FkIntVec2 leftTop;
    FkIntVec2 leftBottom;
    FkIntVec2 rightTop;
    FkIntVec2 rightBottom;
};

#endif //FK_IMAGE_FKCROPPROTO_H