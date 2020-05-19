/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALBITSREADER_H
#define HWVC_ANDROID_ALBITSREADER_H

#include "Object.h"

al_class AlBitsReader al_extend Object {
public:
    AlBitsReader(const uint8_t *data, std::size_t size);

    virtual ~AlBitsReader();

    bool readB();

    uint32_t readU(int num);

    uint32_t readGolombU();

private:
    AlBitsReader(const AlBitsReader &o) : Object() {};

private:
    const uint8_t *data;
    std::size_t size;
    std::size_t posInByte;
    std::size_t posInBit;

};


#endif //HWVC_ANDROID_ALBITSREADER_H
