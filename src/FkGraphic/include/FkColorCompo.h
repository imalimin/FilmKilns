/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: ${author}
* CREATE TIME: 2022-2-25 13:23:24
*/

#ifndef FK_GRAPHIC_FKCOLORCOMPO_H
#define FK_GRAPHIC_FKCOLORCOMPO_H

#include "FkComponent.h"
#include "FkColor.h"

FK_SUPER_CLASS(FkColorCompo, FkComponent) {
FK_DEF_CLASS_TYPE_FUNC(FkColorCompo)

public:
    FkColorCompo(FkColor color);

    FkColorCompo(const FkColorCompo &o);

    virtual ~FkColorCompo();

public:
    FkColor color;
};

#endif //FK_GRAPHIC_FKCOLORCOMPO_H