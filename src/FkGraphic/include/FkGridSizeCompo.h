/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-4-27 0:33:12
*/

#ifndef FK_GRAPHIC_FKGRIDESIZECOMPO_H
#define FK_GRAPHIC_FKGRIDESIZECOMPO_H

#include "FkFloatVec2Compo.h"

FK_CLASS FkGridSizeCompo FK_EXTEND FkFloatVec2Compo {
public:
    FkGridSizeCompo(float x, float y);

    FkGridSizeCompo(const FkGridSizeCompo &o);

    virtual ~FkGridSizeCompo();
};

#endif //FK_GRAPHIC_FKGRIDESIZECOMPO_H