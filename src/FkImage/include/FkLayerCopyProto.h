/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2023-5-19 0:10:49
*/

#ifndef FK_IMAGE_FKLAYERCOPYPROTO_H
#define FK_IMAGE_FKLAYERCOPYPROTO_H

#include "FkProtocol.h"
#include "FkRenderRequest.h"

FK_SUPER_CLASS(FkLayerCopyProto, FkProtocol) {
FK_DEF_CLASS_TYPE_FUNC(FkLayerCopyProto)

public:
    FkLayerCopyProto(FkID src, FkID dst);

    FkLayerCopyProto(const FkLayerCopyProto &o);

    virtual ~FkLayerCopyProto();

public:
    FkID srcLayerId = FK_ID_NONE;
    FkID dstLayerId = FK_ID_NONE;
std::shared_ptr<FkRenderRequest> req = nullptr;
};


#endif //FK_IMAGE_FKLAYERCOPYPROTO_H
