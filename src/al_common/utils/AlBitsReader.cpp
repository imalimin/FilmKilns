/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlBitsReader.h"

#define BITS_PER_BYTE 8

AlBitsReader::AlBitsReader(const uint8_t *data, std::size_t size)
        : Object(), data(data), size(size),
          posInByte(0),
          posInBit(BITS_PER_BYTE - 1) {

}

AlBitsReader::~AlBitsReader() {
    data = nullptr;
}

bool AlBitsReader::readB() {
    if (posInByte >= size) {
        return false;
    }
    bool res = data[posInByte] & 1 << posInBit;
    posInBit--;
    if (posInBit > BITS_PER_BYTE) {
        posInBit = BITS_PER_BYTE - 1;
        posInByte++;
        if (posInByte >= 2) {
            if (data[posInByte - 2] == 0 && data[posInByte - 1] == 0 && data[posInByte] == 3)
                posInByte++;
        }
    }
    return res;
}

uint32_t AlBitsReader::readU(int num) {
    uint32_t result = 0;
    for (int i = 0; i < num; i++)
        if (readB())
            result |= 1 << (num - i - 1);

    return result;
}

uint32_t AlBitsReader::readGolombU() {
    long countOfZero = -1;
    for (bool bit = false; !bit; countOfZero++) {
        bit = readB();
    }
    if (countOfZero >= 32)
        return 0;
    return (1 << countOfZero) - 1 + readU(countOfZero);
}
