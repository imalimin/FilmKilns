/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKSETSURFACEPRT_H
#define FK_GRAPHIC_FKSETSURFACEPRT_H

#include "FkProtocol.h"
#include "FkGraphicWindow.h"

FK_CLASS FkSetSurfacePrt FK_EXTEND FkProtocol {
public:
    FkSetSurfacePrt();

    FkSetSurfacePrt(const FkSetSurfacePrt &o);

    virtual ~FkSetSurfacePrt();

public:
    std::shared_ptr<FkGraphicWindow> win;

};


#endif //FK_GRAPHIC_FKSETSURFACEPRT_H
