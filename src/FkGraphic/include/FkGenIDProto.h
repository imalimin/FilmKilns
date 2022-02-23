/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-22 0:11:10
*/

#ifndef FK_GRAPHIC_FKGENIDPROTO_H
#define FK_GRAPHIC_FKGENIDPROTO_H

#include "FkProtocol.h"

FK_CLASS FkGenIDProto FK_EXTEND FkProtocol {
public:
    FkGenIDProto();

    FkGenIDProto(const FkGenIDProto &o);

    virtual ~FkGenIDProto();

public:
    FkID id = FK_ID_NONE;
};

#endif //FK_GRAPHIC_FKGENIDPROTO_H