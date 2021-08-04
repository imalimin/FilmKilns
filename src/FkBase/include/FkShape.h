/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_BASE_FKSHAPE_H
#define FK_BASE_FKSHAPE_H

#include "FkObject.h"
#include "FkDefinition.h"

FK_CLASS FkShape FK_EXTEND FkObject {
public:
    FK_ENUM kType : int {
        RECT = 0,
        ROUND = 1,
        TRIANGLE = 2,
    };
public:
    FkShape();

    FkShape(const FkShape &o);

    virtual ~FkShape();

    virtual kType getType();

private:
    kType type = kType::RECT;
};

#endif //FK_BASE_FKSHAPE_H