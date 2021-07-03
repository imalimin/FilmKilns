/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKCOORDINATEVALUE_H
#define FK_GRAPHIC_FKCOORDINATEVALUE_H

#include "FkVertexValue.h"

FK_CLASS FkCoordinateValue FK_EXTEND FkVertexValue {
public:
    FkCoordinateValue();

    FkCoordinateValue(const FkCoordinateValue &o);

    virtual ~FkCoordinateValue();
};

#endif //FK_GRAPHIC_FKCOORDINATEVALUE_H