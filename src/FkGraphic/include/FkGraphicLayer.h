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
#include "FkSize.h"

FK_CLASS FkGraphicLayer FK_EXTEND FkGraphicEntity {
public:
    static float calcScaleWithScaleType(std::shared_ptr<FkGraphicLayer> layer,
                                        kScaleType scaleType,
                                        FkSize &targetSize);

public:
    FkGraphicLayer();

    FkGraphicLayer(const FkGraphicLayer &o);

    virtual ~FkGraphicLayer();

    bool operator==(const FkGraphicLayer &obj);

public:
    FkID id = FK_ID_NONE;
};


#endif //FK_GRAPHIC_FKGRAPHICLAYER_H
