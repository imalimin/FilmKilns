/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKCOORDINATECOMPONENT_H
#define FK_GRAPHIC_FKCOORDINATECOMPONENT_H

#include "FkVertexComponent.h"

FK_CLASS FkCoordinateComponent FK_EXTEND FkVertexComponent {
public:
    FkCoordinateComponent();

    FkCoordinateComponent(const FkCoordinateComponent &o);

    virtual ~FkCoordinateComponent();
};

#endif //FK_GRAPHIC_FKCOORDINATECOMPONENT_H