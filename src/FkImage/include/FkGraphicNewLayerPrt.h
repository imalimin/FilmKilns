/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICNEWLAYERPRT_H
#define FK_GRAPHIC_FKGRAPHICNEWLAYERPRT_H

#include "FkLayerProto.h"

FK_SUPER_CLASS(FkGraphicNewLayerPrt, FkLayerProto) {
FK_DEF_CLASS_TYPE_FUNC(FkGraphicNewLayerPrt)

public:
    FkGraphicNewLayerPrt();

    FkGraphicNewLayerPrt(const FkGraphicNewLayerPrt &o);

    virtual ~FkGraphicNewLayerPrt();

public:
    FkID expectId = FK_ID_NONE;

};


#endif //FK_GRAPHIC_FKGRAPHICNEWLAYERPRT_H
