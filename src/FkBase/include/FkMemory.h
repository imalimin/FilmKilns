/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_BASE_FKMEMORY_H
#define FK_BASE_FKMEMORY_H

#include "FkObject.h"

FK_CLASS FkMemory FK_EXTEND FkObject {
public:
    FkMemory();

    FkMemory(const FkMemory &o);

    virtual ~FkMemory();
};

#endif //FK_BASE_FKMEMORY_H