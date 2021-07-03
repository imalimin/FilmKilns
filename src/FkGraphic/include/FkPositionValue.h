/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKPOSITIONVALUE_H
#define FK_GRAPHIC_FKPOSITIONVALUE_H

#include "FkVertexValue.h"

FK_CLASS FkPositionValue FK_EXTEND FkVertexValue {
public:
    FkPositionValue();

    FkPositionValue(const FkPositionValue &o);

    virtual ~FkPositionValue();
};

#endif //FK_GRAPHIC_FKPOSITIONVALUE_H