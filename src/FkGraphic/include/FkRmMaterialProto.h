/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-3-9 0:04:41
*/

#ifndef FK_GRAPHIC_FKRMMATERIALPROTO_H
#define FK_GRAPHIC_FKRMMATERIALPROTO_H

#include "FkMaterialProto.h"

FK_SUPER_CLASS(FkRmMaterialProto, FkMaterialProto) {
FK_DEF_CLASS_TYPE_FUNC(FkRmMaterialProto)

public:
    FkRmMaterialProto();

    FkRmMaterialProto(std::shared_ptr<FkMaterialCompo> &material);

    FkRmMaterialProto(const FkRmMaterialProto &o);

    virtual ~FkRmMaterialProto();

};

#endif //FK_GRAPHIC_FKRMMATERIALPROTO_H