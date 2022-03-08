/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-3-9 0:08:13
*/

#ifndef FK_GRAPHIC_FKMATERIALPROTO_H
#define FK_GRAPHIC_FKMATERIALPROTO_H

#include "FkProtocol.h"
#include "FkMaterialCompo.h"

FK_CLASS FkMaterialProto FK_EXTEND FkProtocol {
public:
    FkMaterialProto(std::shared_ptr<FkMaterialCompo> material);

    FkMaterialProto(const FkMaterialProto &o);

    virtual ~FkMaterialProto();

public:
    std::shared_ptr<FkMaterialCompo> material = nullptr;
};

#endif //FK_GRAPHIC_FKMATERIALPROTO_H