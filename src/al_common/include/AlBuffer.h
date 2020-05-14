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

private:
    AlBuffer(size_t size);

    AlBuffer(uint8_t *refBuf, size_t size);

    AlBuffer(const AlBuffer &e) : Object() {};

public:
    virtual ~AlBuffer();

    uint8_t *data();

    size_t size();

    size_t put(AlBuffer *buf);

    size_t put(uint8_t *data, size_t size);

    size_t get(uint8_t *data, int32_t offset, size_t size);

private:
    uint8_t *buf = nullptr;
    size_t _size = 0;
    bool isRef = false;

};


#endif //HWVC_ANDROID_ALBUFFER_H
