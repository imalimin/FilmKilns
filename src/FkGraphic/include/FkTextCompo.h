/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2023-08-31 13:01:23
*/

#ifndef FK_GRAPHIC_FKTEXTCOMPO_H
#define FK_GRAPHIC_FKTEXTCOMPO_H

#include "FkComponent.h"
#include "FkIntVec2.h"
#include "FkColor.h"

FK_SUPER_CLASS(FkTextCompo, FkComponent) {
FK_DEF_CLASS_TYPE_FUNC(FkTextCompo)
public:
    FkTextCompo(const std::string &text, int32_t textSize, const FkColor &color, const FkIntVec2 &pos);

    FkTextCompo(const FkTextCompo &o);

    virtual ~FkTextCompo();

public:
    std::string text;
    FkIntVec2 pos;
    int32_t textSize;
    FkColor color;
};

#endif //FK_GRAPHIC_FKTEXTCOMPO_H