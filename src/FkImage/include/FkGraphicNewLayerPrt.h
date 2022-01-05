/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICNEWLAYERPRT_H
#define FK_GRAPHIC_FKGRAPHICNEWLAYERPRT_H

#include "FkProtocol.h"
#include "FkGraphicLayer.h"

FK_CLASS FkGraphicNewLayerPrt FK_EXTEND FkProtocol {
public:
    FkGraphicNewLayerPrt();

    FkGraphicNewLayerPrt(const FkGraphicNewLayerPrt &o);

    virtual ~FkGraphicNewLayerPrt();

public:
    std::shared_ptr<FkGraphicLayer> layer = nullptr;

};


#endif //FK_GRAPHIC_FKGRAPHICNEWLAYERPRT_H
