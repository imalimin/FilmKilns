/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_BASE_FKBYTEBUFFER_H
#define FK_BASE_FKBYTEBUFFER_H

#include "FkObject.h"

FK_SUPER_CLASS(FkByteBuffer, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkByteBuffer)

public:
    FkByteBuffer();

    FkByteBuffer(const FkByteBuffer &o);

    virtual ~FkByteBuffer();
};

#endif //FK_BASE_FKBYTEBUFFER_H