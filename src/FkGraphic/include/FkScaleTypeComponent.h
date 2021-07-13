/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKSCALETYPECOMPONENT_H
#define FK_GRAPHIC_FKSCALETYPECOMPONENT_H

#include "FkObject.h"

FK_CLASS FkScaleTypeComponent FK_EXTEND FkObject {
public:
    FkScaleTypeComponent();

    FkScaleTypeComponent(const FkScaleTypeComponent &o);

    virtual ~FkScaleTypeComponent();
};

#endif //FK_GRAPHIC_FKSCALETYPECOMPONENT_H