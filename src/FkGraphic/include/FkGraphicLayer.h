/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICLAYER_H
#define FK_GRAPHIC_FKGRAPHICLAYER_H

#include "FkGraphicEntity.h"
#include "FkBufferComponent.h"

FK_CLASS FkGraphicLayer FK_EXTEND FkGraphicEntity {
public:
    FkGraphicLayer();

    FkGraphicLayer(const FkGraphicLayer &o);

    virtual ~FkGraphicLayer();

public:
    std::shared_ptr<FkBufferComponent> buf = nullptr;
};


#endif //FK_GRAPHIC_FKGRAPHICLAYER_H
