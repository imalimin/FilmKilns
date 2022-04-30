/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-3-13 20:28:34
*/

#ifndef FK_IMAGE_FKFLOATVEC3PROTO_H
#define FK_IMAGE_FKFLOATVEC3PROTO_H

#include "FkProtocol.h"
#include "FkFloatVec3.h"

FK_SUPER_CLASS(FkFloatVec3Proto, FkProtocol) {
FK_DEF_CLASS_TYPE_FUNC(FkFloatVec3Proto)

public:
    FkFloatVec3Proto();

    FkFloatVec3Proto(const FkFloatVec3Proto &o);

    virtual ~FkFloatVec3Proto();

public:
    FkFloatVec3 value;
};

#endif //FK_IMAGE_FKFLOATVEC3PROTO_H