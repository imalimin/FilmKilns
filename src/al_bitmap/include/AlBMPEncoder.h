/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVCSCREEN_ALBMPENCODER_H
#define HWVCSCREEN_ALBMPENCODER_H

#include "AlAbsEncoder.h"

al_class_ex(AlBMPEncoder, AlAbsEncoder) {
public:
    AlBMPEncoder();

    AlBMPEncoder(const AlBMPEncoder &o);

    virtual ~AlBMPEncoder();

    HwResult process(AlBuffer *buf, AlBitmapInfo *info, std::string path) override;

private:
    struct Header {
        uint16_t type;
        uint32_t size;
        uint16_t reserved1;
        uint16_t reserved2;
        uint32_t offset;

        uint32_t Size() {
            return sizeof(type) +
                   sizeof(size) +
                   sizeof(reserved1) +
                   sizeof(reserved2) +
                   sizeof(offset);
        }

        void write(FILE *f) {
            fwrite(&type, 1, sizeof(uint16_t), f);
            fwrite(&size, 1, sizeof(uint32_t), f);
            fwrite(&reserved1, 1, sizeof(uint16_t), f);
            fwrite(&reserved2, 1, sizeof(uint16_t), f);
            fwrite(&offset, 1, sizeof(uint32_t), f);
        }
    };

    struct InfoHeader {
        uint32_t size;
        uint32_t width;
        uint32_t height;
        uint16_t planes;
        uint16_t depthInBit;
        uint32_t compression;
        uint32_t sizeOfImage;
        uint32_t x_pels_per_meter;
        uint32_t y_pels_per_meter;
        uint32_t clr_used;
        uint32_t clr_important;

        uint32_t Size() {
            return sizeof(size) +
                   sizeof(width) +
                   sizeof(height) +
                   sizeof(planes) +
                   sizeof(depthInBit) +
                   sizeof(compression) +
                   sizeof(sizeOfImage) +
                   sizeof(x_pels_per_meter) +
                   sizeof(y_pels_per_meter) +
                   sizeof(clr_used) +
                   sizeof(clr_important);
        }

        void write(FILE *f) {
            fwrite(&size, 1, sizeof(uint32_t), f);
            fwrite(&width, 1, sizeof(uint32_t), f);
            fwrite(&height, 1, sizeof(uint32_t), f);
            fwrite(&planes, 1, sizeof(uint16_t), f);
            fwrite(&depthInBit, 1, sizeof(uint16_t), f);

            fwrite(&compression, 1, sizeof(uint32_t), f);
            fwrite(&sizeOfImage, 1, sizeof(uint32_t), f);
            fwrite(&x_pels_per_meter, 1, sizeof(uint32_t), f);
            fwrite(&y_pels_per_meter, 1, sizeof(uint32_t), f);
            fwrite(&clr_used, 1, sizeof(uint32_t), f);
            fwrite(&clr_important, 1, sizeof(uint32_t), f);
        }
    };

};


#endif //HWVCSCREEN_ALBMPENCODER_H
