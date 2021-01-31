/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICTEXPTL_H
#define FK_GRAPHIC_FKGRAPHICTEXPTL_H

#include "FkProtocol.h"

FK_CLASS FkGraphicTexPtl FK_EXTEND FkProtocol {
public:
    FkGraphicTexPtl();

    FkGraphicTexPtl(const FkGraphicTexPtl &o);

    virtual ~FkGraphicTexPtl();

public:
    FkID texId = FK_ID_NONE;

};


#endif //FK_GRAPHIC_FKGRAPHICTEXPTL_H
