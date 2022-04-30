/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICENTITY_H
#define FK_GRAPHIC_FKGRAPHICENTITY_H

#include "FkEntity.h"
#include "FkGraphicComponent.h"
#include <vector>
#include <list>

FK_SUPER_CLASS(FkGraphicEntity, FkEntity) {
FK_DEF_CLASS_TYPE_FUNC(FkGraphicEntity)

public:
    FkGraphicEntity();

    FkGraphicEntity(const FkGraphicEntity &o);

    virtual ~FkGraphicEntity();
};


#endif //FK_GRAPHIC_FKGRAPHICENTITY_H
