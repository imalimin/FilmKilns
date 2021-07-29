/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKCOLORVALUE_H
#define FK_GRAPHIC_FKCOLORVALUE_H

#include "FkObject.h"
#include "FkColor.h"

FK_CLASS FkColorValue FK_EXTEND FkObject {
public:
    FkColorValue();

    FkColorValue(const FkColorValue &o);

    virtual ~FkColorValue();

public:
    FkColor color;
};

#endif //FK_GRAPHIC_FKCOLORVALUE_H