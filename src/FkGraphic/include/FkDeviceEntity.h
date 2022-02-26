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

FK_CLASS FkDeviceEntity FK_EXTEND FkMaterialEntity {
public:
    FkDeviceEntity(std::shared_ptr<FkMaterialCompo> material);

    FkDeviceEntity(const FkDeviceEntity &o);

    virtual ~FkDeviceEntity();
};

FK_CLASS FkBufDeviceEntity FK_EXTEND FkDeviceEntity {
public:
    FkBufDeviceEntity(std::shared_ptr<FkBuffer> &buf);

    FkBufDeviceEntity(const FkBufDeviceEntity &o);

    virtual ~FkBufDeviceEntity();

    std::shared_ptr<FkBuffer> buffer();

    void finish();
};

#endif //FK_GRAPHIC_FKDEVICEENTITY_H