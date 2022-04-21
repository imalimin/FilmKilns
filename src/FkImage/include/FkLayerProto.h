/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-4-19 23:37:02
*/

#ifndef FK_IMAGE_FKLAYERPROTO_H
#define FK_IMAGE_FKLAYERPROTO_H

#include "FkProtocol.h"
#include "FkGraphicLayer.h"

FK_CLASS FkLayerProto FK_EXTEND FkProtocol {
public:
    FkLayerProto();

    FkLayerProto(const FkLayerProto &o);

    virtual ~FkLayerProto();
public:
    FkID layerId = FK_ID_NONE;
    std::shared_ptr<FkGraphicLayer> layer = nullptr;
};

#endif //FK_IMAGE_FKLAYERPROTO_H