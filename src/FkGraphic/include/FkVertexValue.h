/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKVERTEXVALUE_H
#define FK_GRAPHIC_FKVERTEXVALUE_H

#include "FkProgramValue.h"

FK_CLASS FkVertexValue FK_EXTEND FkProgramValue {
public:
    FkVertexValue();

    FkVertexValue(const FkVertexValue &o);

    virtual ~FkVertexValue();

public:
    size_t countVertex = 0;
    size_t countPerVertex = 0;
    size_t offset = 0;
    void *data = nullptr;
};

#endif //FK_GRAPHIC_FKVERTEXVALUE_H