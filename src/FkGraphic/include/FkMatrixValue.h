/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKMATRIXVALUE_H
#define FK_GRAPHIC_FKMATRIXVALUE_H

#include "FkProgramValue.h"
#include "FkMatrix.h"

FK_CLASS FkMatrixValue FK_EXTEND FkProgramValue {
public:
    FkMatrixValue();

    FkMatrixValue(const FkMatrixValue &o);

    virtual ~FkMatrixValue();

public:
    std::shared_ptr<FkMatrix> mat = nullptr;
    int32_t index = 0;
};

#endif //FK_GRAPHIC_FKMATRIXVALUE_H