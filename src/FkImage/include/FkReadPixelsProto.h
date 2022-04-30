/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-4-19 23:37:24
*/

#ifndef FK_IMAGE_FKREADPIXELSPROTO_H
#define FK_IMAGE_FKREADPIXELSPROTO_H

#include "FkLayerProto.h"
#include "FkBuffer.h"

FK_SUPER_CLASS(FkReadPixelsProto, FkLayerProto) {
FK_DEF_CLASS_TYPE_FUNC(FkReadPixelsProto)

public:
    FkReadPixelsProto();

    FkReadPixelsProto(const FkReadPixelsProto &o);

    virtual ~FkReadPixelsProto();

public:
    FkSize size;
    std::shared_ptr<FkBuffer> buf = nullptr;
};

#endif //FK_IMAGE_FKREADPIXELSPROTO_H