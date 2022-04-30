/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-26 11:47:54
*/

#ifndef FK_GRAPHIC_FKDEVICEENTITY_H
#define FK_GRAPHIC_FKDEVICEENTITY_H

#include "FkMaterialEntity.h"
#include "FkBuffer.h"
#include "FkTexCompo.h"
#include "FkSize.h"

FK_SUPER_CLASS(FkDeviceEntity, FkMaterialEntity) {
FK_DEF_CLASS_TYPE_FUNC(FkDeviceEntity)

public:
    FkDeviceEntity(std::shared_ptr<FkMaterialCompo> material);

    FkDeviceEntity(const FkDeviceEntity &o);

    virtual ~FkDeviceEntity();
};

FK_SUPER_CLASS(FkBufDeviceEntity, FkDeviceEntity) {
FK_DEF_CLASS_TYPE_FUNC(FkBufDeviceEntity)

public:
    FkBufDeviceEntity(std::shared_ptr<FkBuffer> &buf);

    FkBufDeviceEntity(const FkBufDeviceEntity &o);

    virtual ~FkBufDeviceEntity();

    std::shared_ptr<FkBuffer> buffer();

    void finish();
};

FK_SUPER_CLASS(FkTexDeviceEntity, FkDeviceEntity) {
FK_DEF_CLASS_TYPE_FUNC(FkTexDeviceEntity)

public:
    FkTexDeviceEntity(std::shared_ptr<FkMaterialCompo> material);

    FkTexDeviceEntity(const FkTexDeviceEntity &o);

    virtual ~FkTexDeviceEntity();

    void finish();

    /**
     * @NotNull
     * @return
     */
    std::shared_ptr<FkTexCompo> tex();

    FkSize size();

    FkColor::kFormat format();
};

FK_SUPER_CLASS(FkScreenEntity, FkDeviceEntity) {
FK_DEF_CLASS_TYPE_FUNC(FkScreenEntity)

public:
    FkScreenEntity();

    FkScreenEntity(const FkTexDeviceEntity &o);

    virtual ~FkScreenEntity();

    FkSize size();
};

#endif //FK_GRAPHIC_FKDEVICEENTITY_H