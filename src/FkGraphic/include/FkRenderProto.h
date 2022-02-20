/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKRENDERPROTO_H
#define FK_GRAPHIC_FKRENDERPROTO_H

#include "FkProtocol.h"
#include "FkEnvEntity.h"
#include "FkMaterialEntity.h"
#include "FkTransEntity.h"

FK_CLASS FkRenderProto FK_EXTEND FkProtocol {
public:
    FkRenderProto();

    FkRenderProto(const FkRenderProto &o);

    virtual ~FkRenderProto();

public:
    std::shared_ptr<FkEnvEntity> env = std::make_shared<FkEnvEntity>();
    std::shared_ptr<FkMaterialEntity> material = std::make_shared<FkMaterialEntity>();
    std::shared_ptr<FkTransEntity> trans = std::make_shared<FkTransEntity>();
};

#endif //FK_GRAPHIC_FKRENDERPROTO_H