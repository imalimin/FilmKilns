/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICLAYERPRT_H
#define FK_GRAPHIC_FKGRAPHICLAYERPRT_H

#include "FkProtocol.h"
#include "FkGraphicLayer.h"

FK_CLASS FkGraphicLayerPrt FK_EXTEND FkProtocol {
public:
    FkGraphicLayerPrt();

    FkGraphicLayerPrt(const FkGraphicLayerPrt &o);

    virtual ~FkGraphicLayerPrt();

public:
    std::shared_ptr<FkGraphicLayer> layer = nullptr;

};


#endif //FK_GRAPHIC_FKGRAPHICLAYERPRT_H
