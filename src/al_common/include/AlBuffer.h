/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_ALBUFFER_H
#define HWVC_ANDROID_ALBUFFER_H

#include "Object.h"

al_class AlBuffer al_extend Object {
public:
    static AlBuffer *alloc(size_t size);

    static AlBuffer *wrap(uint8_t *buf, size_t size);

    static AlBuffer *wrap(AlBuffer *buf);

private:
    AlBuffer(size_t size);

    AlBuffer(uint8_t *refBuf, size_t size);

    AlBuffer(const AlBuffer &o);

    void _movePosition(size_t offset);

public:
    virtual ~AlBuffer();

    uint8_t *data();

    size_t size();

    void rewind();

    size_t remaining();

    size_t capacity();

    void limit(size_t newLimit);

    size_t put(AlBuffer *buf);

    size_t put(uint8_t *data, size_t size);

    ///
    /// \param dst dest buffer pointer
    /// \param size request read count of byte.
    /// \return zero for fail, or size for success.
    size_t get(uint8_t *dst, size_t size);

    short getShort();

    float getFloat();

private:
    uint8_t *buf = nullptr;
    size_t _size = 0;
    size_t _position = 0;
    size_t _limit = 0;
    size_t _capacity = 0;
    bool isRef = false;

};


#endif //HWVC_ANDROID_ALBUFFER_H
