/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "AlBMPEncoder.h"
#include "AlLogcat.h"

#define TAG "AlBMPEncoder"

AlBMPEncoder::AlBMPEncoder() : AlAbsEncoder() {

}

AlBMPEncoder::AlBMPEncoder(const AlBMPEncoder &o) : AlAbsEncoder() {

}

AlBMPEncoder::~AlBMPEncoder() {

}

HwResult AlBMPEncoder::process(AlBuffer *buf, AlBitmapInfo *info, std::string path) {
    if (nullptr == buf || nullptr == info || info->depth <= 0) {
        AlLogE(TAG, "failed");
        return Hw::FAILED;
    }
//    for (int i = 0; i < info->height; ++i) {
//        for (int j = 0; j < info->width; ++j) {
//            buf->data()[i * info->width * 4 + j * 4 + 0] = 0;
//            buf->data()[i * info->width * 4 + j * 4 + 1] = 0;
//            buf->data()[i * info->width * 4 + j * 4 + 2] = 0;
//            buf->data()[i * info->width * 4 + j * 4 + 3] = 0;
//        }
//    }
    uint16_t depthInBit = static_cast<uint16_t>(AlBitmapInfo::bitsPerPixel(info->colorSpace));
    Header header = {0, 0, 0, 0, 0};
    InfoHeader infoHeader = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    header.type = 0x4d42;
    header.size = sizeof(header) + sizeof(infoHeader) + buf->size();
    header.reserved1 = 0;
    header.reserved2 = 0;
    header.offset = header.Size() + infoHeader.Size();

    infoHeader.size = 40;
    infoHeader.width = static_cast<uint32_t>(info->width);
    infoHeader.height = static_cast<uint32_t>(info->height);
    infoHeader.planes = 1;
    infoHeader.depthInBit = depthInBit;
    /// RGB:0(不压缩)
    infoHeader.compression = 0;
    /// (info->width * (depthInBit / 8) * 8u + 31u) / 32u * 4u * info->height
    /// 不压缩时可以为0
    infoHeader.sizeOfImage = 0;
    infoHeader.x_pels_per_meter = 0;
    infoHeader.y_pels_per_meter = 0;
    infoHeader.clr_used = 0;
    infoHeader.clr_important = 0;
    AlLogI(TAG, "depthInBit %d", infoHeader.depthInBit);

    FILE *file = fopen(path.c_str(), "wb");
    if (nullptr == file) {
        AlLogE(TAG, "failed. Could not open file %s", path.c_str());
        return Hw::FAILED;
    }
    header.write(file);
    infoHeader.write(file);
    fwrite(buf->data(), 1, buf->size(), file);
    fclose(file);
    return Hw::SUCCESS;
}
