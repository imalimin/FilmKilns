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

FK_CLASS FkRenderRequest FK_EXTEND FkGraphicEntity {
public:
    FkRenderRequest();

    FkRenderRequest(const FkRenderRequest &o);

    virtual ~FkRenderRequest();

public:
    std::shared_ptr<FkGraphicLayer> canvas;
    std::vector<std::shared_ptr<FkGraphicLayer>> layers;
};

#endif //FK_GRAPHIC_FKRENDERREQUEST_H