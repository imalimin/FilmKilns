/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKMATRIXCOMPONENT_H
#define FK_GRAPHIC_FKMATRIXCOMPONENT_H

#include "FkGraphicComponent.h"

FK_CLASS FkMatrixComponent FK_EXTEND FkGraphicComponent {
public:
    FkMatrixComponent();

    FkMatrixComponent(const FkMatrixComponent &o);

    virtual ~FkMatrixComponent();
};

#endif //FK_GRAPHIC_FKMATRIXCOMPONENT_H