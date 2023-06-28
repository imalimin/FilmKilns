/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-3-13 20:28:16
*/

#ifndef FK_IMAGE_FKRATIONALPROTO_H
#define FK_IMAGE_FKRATIONALPROTO_H

#include "FkProtocol.h"
#include "FkRational.h"

FK_SUPER_CLASS(FkRationalProto, FkProtocol) {
FK_DEF_CLASS_TYPE_FUNC(FkRationalProto)

public:
    FkRationalProto();

    FkRationalProto(const FkRationalProto &o);

    virtual ~FkRationalProto();

public:
    FkRational value;
};

#endif //FK_IMAGE_FKRATIONALPROTO_H