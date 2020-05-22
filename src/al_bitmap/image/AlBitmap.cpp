/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "AlBitmap.h"
#include "Logcat.h"

AlBitmap *AlBitmap::create(AlBitmapInfo info) {
    if (AlColorFormat::NONE == info.colorFormat) {
        Logcat::e("HWVC", "Invalid image color space!");
        return nullptr;
    }
    return new AlBitmap(info);
}

AlBitmap *AlBitmap::create(int width, int height, AlColorFormat colorFormat) {
    if (AlColorFormat::NONE == colorFormat) {
        Logcat::e("HWVC", "Invalid image color space!");
        return nullptr;
    }
    return new AlBitmap(width, height, 8, colorFormat);
}

float AlBitmap::getImageFormatBytes(AlColorFormat format) {
    switch (format) {
        case AlColorFormat::RGB:
            return 3;
        case AlColorFormat::RGBA:
            return 4;
        case AlColorFormat::NV12:
        case AlColorFormat::YV12:
            return 1.5;
        case AlColorFormat::GRAY:
            return 1;
        default:
            return 0;
    }
}

AlBitmap::AlBitmap(AlBitmapInfo &info) {
    this->info = info;
    config();
}

AlBitmap::AlBitmap(int width, int height, uint32_t depth, AlColorFormat colorFormat) {
    this->info.width = width;
    this->info.height = height;
    this->info.depth = depth;
    this->info.colorFormat = colorFormat;
    config();
}

void AlBitmap::config() {
    if (AlColorFormat::NONE == info.colorFormat) {
        Logcat::e("HWVC", "Invalid image format!");
        return;
    }
    byteSize = static_cast<uint64_t>(getByteSize());
    pixels = new uint8_t[byteSize];
}

AlBitmap::~AlBitmap() {
    if (pixels) {
        delete[]pixels;
        pixels = nullptr;
    }
    byteSize = 0;
    info.width = 0;
    info.height = 0;
    info.colorFormat = AlColorFormat::NONE;
}

int AlBitmap::getWidth() { return info.width; }

int AlBitmap::getHeight() { return info.height; }

HwResult AlBitmap::resize(int width, int height, AlColorFormat colorFormat) {
    if (AlColorFormat::NONE == colorFormat) {
        Logcat::e("HWVC", "Invalid image format!");
        return Hw::FAILED;
    }
    int64_t byteSize = static_cast<uint64_t>(width * height * getImageFormatBytes(colorFormat));
    if (byteSize > this->byteSize) {
        return Hw::FAILED;
    }
    this->info.width = width;
    this->info.height = height;
    this->info.colorFormat = colorFormat;
    if (!this->pixels) {
        this->byteSize = getByteSize();
        this->pixels = new uint8_t[this->byteSize];
    }
    return Hw::SUCCESS;
}

uint8_t *AlBitmap::getPixels() { return pixels; }

uint64_t AlBitmap::getByteSize() {
    return static_cast<uint64_t>(info.width * info.height * getImageFormatBytes(info.colorFormat));
}

AlRational AlBitmap::getRotation() {
    return info.rotation;
}

void AlBitmap::dump() {
    info.dump();
}
