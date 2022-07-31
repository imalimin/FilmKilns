/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2022-07-31 10:33:21
*/

#ifndef FK_IMAGE_FKQUERYLAYERPROTO_H
#define FK_IMAGE_FKQUERYLAYERPROTO_H

#include "FkProtocol.h"
#include "FkGraphicLayer.h"

FK_SUPER_CLASS(FkQueryLayerProto, FkProtocol) {
FK_DEF_CLASS_TYPE_FUNC(FkQueryLayerProto)

public:
    FkQueryLayerProto(FkID layerId);

    FkQueryLayerProto(const FkQueryLayerProto &o);

    virtual ~FkQueryLayerProto();

public:
    FkID layerId = FK_ID_NONE;
    std::shared_ptr<FkGraphicLayer> layer = nullptr;
};

#endif //FK_IMAGE_FKQUERYLAYERPROTO_H