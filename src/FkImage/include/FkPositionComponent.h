/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKPOSITIONCOMPONENT_H
#define FK_GRAPHIC_FKPOSITIONCOMPONENT_H

#include "FkVertexComponent.h"

FK_CLASS FkPositionComponent FK_EXTEND FkVertexComponent {
public:
    FkPositionComponent();

    FkPositionComponent(const FkPositionComponent &o);

    virtual ~FkPositionComponent();
};

#endif //FK_GRAPHIC_FKPOSITIONCOMPONENT_H