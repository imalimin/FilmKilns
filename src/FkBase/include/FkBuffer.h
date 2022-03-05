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
    static std::shared_ptr<FkBuffer> wrap(uint8_t *data, size_t size);

    static std::shared_ptr<FkBuffer> alloc(size_t size);

public:
    FkBuffer(const FkBuffer &o) = delete;

    virtual ~FkBuffer();

    size_t capacity();

    uint8_t *data() const;

private:
    FkBuffer(uint8_t *data, size_t size);

    FkBuffer(size_t size);

private:
    uint8_t *_data = nullptr;
    size_t _capacity = 0;
    bool isRef = false;
};

#endif //FK_BASE_FKBUFFER_H