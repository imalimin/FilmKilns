/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICUPDATECANVASPROTO_H
#define FK_GRAPHIC_FKGRAPHICUPDATECANVASPROTO_H

#include "FkGraphicUpdateLayerPrt.h"

FK_CLASS FkGraphicUpdateCanvasProto FK_EXTEND FkGraphicUpdateLayerPrt {
public:
    FkGraphicUpdateCanvasProto();

    FkGraphicUpdateCanvasProto(const FkGraphicUpdateCanvasProto &o);

    virtual ~FkGraphicUpdateCanvasProto();
};

#endif //FK_GRAPHIC_FKGRAPHICUPDATECANVASPROTO_H