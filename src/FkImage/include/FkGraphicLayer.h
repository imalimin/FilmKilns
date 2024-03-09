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
#include "FkMaterialCompo.h"
#include "FkFloatVec3.h"
#include "FkIntVec2.h"
#include "FkRational.h"

FK_SUPER_CLASS(FkGraphicLayer, FkGraphicEntity) {
FK_DEF_CLASS_TYPE_FUNC(FkGraphicLayer)

public:
    static float calcScaleWithScaleType(FkSize &src,
                                        FkSize &dst,
                                        kScaleType scaleType);

public:
    FkGraphicLayer();

    FkGraphicLayer(const FkGraphicLayer &o);

    virtual ~FkGraphicLayer();

    bool operator==(const FkGraphicLayer &obj);

    FkSize getSize();

    FkFloatVec3 getScale();

    FkIntVec2 getTrans();

    FkRational getRotate();

    int getVisibility();

    std::string toString() override;

public:
    FkID id = FK_ID_NONE;
    /// Projection layer ID
    FkID projLayerId = FK_ID_NONE;
    int32_t zIndex = 0;
    std::shared_ptr<FkMaterialCompo> material = nullptr;
};


#endif //FK_GRAPHIC_FKGRAPHICLAYER_H
