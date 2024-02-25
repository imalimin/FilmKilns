/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2024-02-25 15:36:45
*/

#ifndef FK_IMAGE_FKLAYERSETPROJECTIONPROTO_H
#define FK_IMAGE_FKLAYERSETPROJECTIONPROTO_H

#include "FkProtocol.h"

FK_SUPER_CLASS(FkLayerSetProjectionProto, FkProtocol) {
FK_DEF_CLASS_TYPE_FUNC(FkLayerSetProjectionProto)

public:
    FkLayerSetProjectionProto(FkID srcLayerId, FkID dstLayerId);

    FkLayerSetProjectionProto(const FkLayerSetProjectionProto &o);

    virtual ~FkLayerSetProjectionProto();

public:
    FkID srcLayerId = FK_ID_NONE;
    FkID dstLayerId = FK_ID_NONE;
};

#endif //FK_IMAGE_FKLAYERSETPROJECTIONPROTO_H