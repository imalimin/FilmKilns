/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-3-13 13:38:23
*/

#ifndef FK_IMAGE_FKINTVEC2PROTO_H
#define FK_IMAGE_FKINTVEC2PROTO_H

#include "FkProtocol.h"
#include "FkDoubleVec2.h"

FK_SUPER_CLASS(FkIntVec2Proto, FkProtocol) {
FK_DEF_CLASS_TYPE_FUNC(FkIntVec2Proto)

public:
    FkIntVec2Proto();

    FkIntVec2Proto(const FkIntVec2Proto &o);

    virtual ~FkIntVec2Proto();

public:
    FkDoubleVec2 value;
};

#endif //FK_IMAGE_FKINTVEC2PROTO_H