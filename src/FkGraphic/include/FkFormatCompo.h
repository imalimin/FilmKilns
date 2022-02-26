/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-26 0:23:57
*/

#ifndef FK_GRAPHIC_FKFORMATCOMPO_H
#define FK_GRAPHIC_FKFORMATCOMPO_H

#include "FkComponent.h"
#include "FkColor.h"

FK_CLASS FkFormatCompo FK_EXTEND FkComponent {
public:
    FkFormatCompo(FkColor::kFormat fmt);

    FkFormatCompo(const FkFormatCompo &o);

    virtual ~FkFormatCompo();

public:
    FkColor::kFormat fmt = FkColor::kFormat::NONE;
};

#endif //FK_GRAPHIC_FKFORMATCOMPO_H