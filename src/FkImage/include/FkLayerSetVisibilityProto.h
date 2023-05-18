/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2023-5-19 0:10:49
*/

#ifndef FK_IMAGE_FKLAYERSETVISIBILITYPROTO_H
#define FK_IMAGE_FKLAYERSETVISIBILITYPROTO_H

#include "FkProtocol.h"

FK_SUPER_CLASS(FkLayerSetVisibilityProto, FkProtocol) {
FK_DEF_CLASS_TYPE_FUNC(FkLayerSetVisibilityProto)

public:
    FkLayerSetVisibilityProto();

    FkLayerSetVisibilityProto(const FkLayerSetVisibilityProto &o);

    virtual ~FkLayerSetVisibilityProto();

public:
    FkID layerId = FK_ID_NONE;
    int visibility = FK_VISIBLE;
};


#endif //FK_IMAGE_FKLAYERSETVISIBILITYPROTO_H
