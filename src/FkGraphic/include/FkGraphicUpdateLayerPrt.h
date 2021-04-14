/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICUPDATELAYERPRT_H
#define FK_GRAPHIC_FKGRAPHICUPDATELAYERPRT_H

#include "FkProtocol.h"
#include "FkGraphicLayer.h"

FK_CLASS FkGraphicUpdateLayerPrt FK_EXTEND FkProtocol {
public:
    FkGraphicUpdateLayerPrt();

    FkGraphicUpdateLayerPrt(const FkGraphicUpdateLayerPrt &o);

    virtual ~FkGraphicUpdateLayerPrt();

public:
    std::shared_ptr<FkGraphicLayer> layer = nullptr;

};


#endif //FK_GRAPHIC_FKGRAPHICUPDATELAYERPRT_H
