/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_BASE_FKBUFFER_H
#define FK_BASE_FKBUFFER_H

#include "FkObject.h"

FK_SUPER_CLASS(FkBuffer, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkBuffer)

public:
    static std::shared_ptr<FkBuffer> wrap(uint8_t *data, size_t size);

    static std::shared_ptr<FkBuffer> alloc(size_t size);

public:
    FkBuffer(const FkBuffer &o) = delete;

    virtual ~FkBuffer();

    size_t capacity();

    size_t size();

    uint8_t *data() const;

    size_t put(uint8_t *data, size_t size);

    size_t remaining();

    void rewind();

    short getShort();

    float getFloat();

private:
    FkBuffer(uint8_t *data, size_t size);

    FkBuffer(size_t size);

    void _movePosition(size_t offset);

private:
    uint8_t *_data = nullptr;
    size_t _capacity = 0;
    size_t _limit = 0;
    size_t _position = 0;
    bool isRef = false;
};

#endif //FK_BASE_FKBUFFER_H