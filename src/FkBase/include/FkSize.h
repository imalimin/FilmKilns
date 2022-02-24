/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_BASE_FKSIZE_H
#define FK_BASE_FKSIZE_H

#include "FkObject.h"

FK_CLASS FkSize FK_EXTEND FkObject {
public:
    FkSize();

    FkSize(int32_t width, int32_t height);

    FkSize(int64_t size);

    FkSize(const FkSize &o);

    virtual ~FkSize();

    virtual void set(int32_t _width, int32_t _height);

    virtual int32_t getWidth();

    virtual int32_t getHeight();

    virtual bool operator==(const FkSize &o);

    virtual bool operator!=(const FkSize &o);

    virtual bool isZero();

    int64_t toInt64();

private:
    int32_t width = 0;
    int32_t height = 0;
};


#endif //FK_BASE_FKSIZE_H
