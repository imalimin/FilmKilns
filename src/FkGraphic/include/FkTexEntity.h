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
#include "FkTexCompo.h"
#include "FkFboCompo.h"
#include "FkSize.h"
#include "FkColor.h"

FK_CLASS FkTexEntity FK_EXTEND FkMaterialEntity {
public:
    FkTexEntity(std::shared_ptr<FkMaterialCompo> &material);

    FkTexEntity(std::shared_ptr<FkMaterialCompo> &material, std::shared_ptr<FkTexCompo> &tex);

    FkTexEntity(const FkTexEntity &o);

    FkTexEntity(const FkMaterialEntity &o);

    virtual ~FkTexEntity();

    /**
     * @NotNull
     * @return
     */
    std::shared_ptr<FkTexCompo> tex();

    /**
     * @Nullbale
     * @return
     */
    std::shared_ptr<FkFboCompo> fbo();

    FkSize size();

    FkColor::kFormat format();

};

#endif //FK_GRAPHIC_FKTEXENTITY_H