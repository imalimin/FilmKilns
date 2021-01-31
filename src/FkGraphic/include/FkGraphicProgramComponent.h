/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICPROGRAMCOMPONENT_H
#define FK_GRAPHIC_FKGRAPHICPROGRAMCOMPONENT_H

#include "FkGraphicComponent.h"
#include "FkGraphicProgram.h"

class FkGraphicProgramComponent FK_EXTEND FkGraphicComponent {
public:
    FkGraphicProgramComponent();

    FkGraphicProgramComponent(const FkGraphicProgramComponent &o);

    virtual ~FkGraphicProgramComponent();

public:
    std::shared_ptr<FkGraphicProgram> program = nullptr;
};


#endif //FK_GRAPHIC_FKGRAPHICPROGRAMCOMPONENT_H
