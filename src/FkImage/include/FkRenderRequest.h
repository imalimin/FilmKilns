/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKRENDERREQUEST_H
#define FK_GRAPHIC_FKRENDERREQUEST_H

#include "FkGraphicEntity.h"
#include "FkGraphicLayer.h"

FK_SUPER_CLASS(FkRenderRequest, FkGraphicEntity) {
FK_DEF_CLASS_TYPE_FUNC(FkRenderRequest)

public:
    FkRenderRequest();

    FkRenderRequest(const FkRenderRequest &o);

    virtual ~FkRenderRequest();

    std::shared_ptr<FkGraphicLayer> getCanvas();

    std::shared_ptr<FkGraphicLayer> findLayerById(FkID layerId);

    std::string toString() override;

public:
    std::vector<std::shared_ptr<FkGraphicLayer>> layers;
};

#endif //FK_GRAPHIC_FKRENDERREQUEST_H