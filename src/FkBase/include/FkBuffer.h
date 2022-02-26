/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_BASE_FKBUFFER_H
#define FK_BASE_FKBUFFER_H

#include "FkObject.h"

FK_CLASS FkBuffer FK_EXTEND FkObject {
public:
    FkBuffer(size_t size);

    FkBuffer(const FkBuffer &o);

    virtual ~FkBuffer();

    size_t capacity();

    uint8_t *data() const;

private:
    uint8_t *_data = nullptr;
    size_t _capacity = 0;
};

#endif //FK_BASE_FKBUFFER_H