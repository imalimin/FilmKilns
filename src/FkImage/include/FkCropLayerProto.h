/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2024-03-09 16:19:26
*/

#ifndef FK_IMAGE_FKCROPLAYERPROTO_H
#define FK_IMAGE_FKCROPLAYERPROTO_H

#include "FkProtocol.h"
#include "FkRect.h"

FK_SUPER_CLASS(FkCropLayerProto, FkProtocol) {
FK_DEF_CLASS_TYPE_FUNC(FkCropLayerProto)

public:
    FkCropLayerProto(const FkID layerId, const FkIntRect &rect);

    FkCropLayerProto(const FkCropLayerProto &o);

    virtual ~FkCropLayerProto();

public:
    FkID layerId = FK_ID_NONE;
    FkIntRect rect;
};

#endif //FK_IMAGE_FKCROPLAYERPROTO_H