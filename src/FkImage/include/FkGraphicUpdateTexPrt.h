/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICUPDATETEXPRT_H
#define FK_GRAPHIC_FKGRAPHICUPDATETEXPRT_H

#include "FkObject.h"
#include "FkProtocol.h"
#include "FkSize.h"

FK_CLASS FkGraphicUpdateTexPrt FK_EXTEND FkProtocol {
public:
    FkGraphicUpdateTexPrt();

    FkGraphicUpdateTexPrt(const FkGraphicUpdateTexPrt &o);

    virtual ~FkGraphicUpdateTexPrt();

public:
    FkID id = FK_ID_NONE;
    FkSize size;
};

#endif //FK_GRAPHIC_FKGRAPHICUPDATETEXPRT_H