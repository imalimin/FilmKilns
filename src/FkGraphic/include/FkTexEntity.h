/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: ${author}
* CREATE TIME: 2022-2-25 13:24:09
*/

#ifndef FK_GRAPHIC_FKTEXENTITY_H
#define FK_GRAPHIC_FKTEXENTITY_H

#include "FkMaterialEntity.h"
#include "FkSizeCompo.h"
#include "FkTexCompo.h"

FK_CLASS FkTexEntity FK_EXTEND FkMaterialEntity {
public:
    FkTexEntity();

    FkTexEntity(const FkTexEntity &o);

    virtual ~FkTexEntity();

    std::shared_ptr<FkTexCompo> tex();

    FkSize size();

};

#endif //FK_GRAPHIC_FKTEXENTITY_H