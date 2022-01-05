/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICTEXPTL_H
#define FK_GRAPHIC_FKGRAPHICTEXPTL_H

#include "FkProtocol.h"
#include "FkColor.h"

FK_CLASS FkGraphicTexPtl FK_EXTEND FkProtocol {
public:
    FkGraphicTexPtl();

    FkGraphicTexPtl(const FkGraphicTexPtl &o);

    virtual ~FkGraphicTexPtl();

public:
    FkID id = FK_ID_NONE;
    FkColor::kFormat fmt = FkColor::kFormat::NONE;
};


#endif //FK_GRAPHIC_FKGRAPHICTEXPTL_H
