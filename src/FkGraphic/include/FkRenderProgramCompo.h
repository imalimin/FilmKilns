/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-27 15:59:08
*/

#ifndef FK_GRAPHIC_FKRENDERPROGRAMCOMPO_H
#define FK_GRAPHIC_FKRENDERPROGRAMCOMPO_H

#include "FkComponent.h"
#include "FkGraphicProgram.h"

FK_SUPER_CLASS(FkRenderProgramCompo, FkComponent) {
FK_DEF_CLASS_TYPE_FUNC(FkRenderProgramCompo)

public:
    FkRenderProgramCompo();

    FkRenderProgramCompo(const FkRenderProgramCompo &o);

    virtual ~FkRenderProgramCompo();

public:
    std::shared_ptr<FkGraphicProgram> program = nullptr;
};

#endif //FK_GRAPHIC_FKRENDERPROGRAMCOMPO_H