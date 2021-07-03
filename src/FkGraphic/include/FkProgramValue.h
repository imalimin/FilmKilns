/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKPROGRAMVALUE_H
#define FK_GRAPHIC_FKPROGRAMVALUE_H

#include "FkObject.h"

FK_CLASS FkProgramValue FK_EXTEND FkObject {
public:
    FkProgramValue();

    FkProgramValue(const FkProgramValue &o);

    virtual ~FkProgramValue();
};

#endif //FK_GRAPHIC_FKPROGRAMVALUE_H