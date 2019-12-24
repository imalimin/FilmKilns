/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "AlBitmap.h"
#include "Logcat.h"

AlBitmap *AlBitmap::create(AlBitmapInfo info) {
    if (AlColorSpace::NONE == info.colorSpace) {
        Logcat::e("HWVC", "Invalid image color space!");
        return nullptr;
    }
    return new AlBitmap(info);
}

AlBitmap *AlBitmap::create(int width, int height, AlColorSpace colorSpace) {
    if (AlColorSpace::NONE == colorSpace) {
        Logcat::e("HWVC", "Invalid image color space!");
        return nullptr;
    }
    return new AlBitmap(width, height, 8, colorSpace);
}

float AlBitmap::getImageFormatBytes(AlColorSpace format) {
    switch (format) {
        case AlColorSpace::RGB:
            return 3;
        case AlColorSpace::RGBA:
            return 4;
        case AlColorSpace::NV12:
        case AlColorSpace::YV12:
            return 1.5;
        case AlColorSpace::GRAY:
            return 1;
        default:
            return 0;
    }
}

AlBitmap::AlBitmap(AlBitmapInfo &info) {
    this->info = info;
    config();
}

AlBitmap::AlBitmap(int width, int height, uint32_t depth, AlColorSpace colorSpace) {
    this->info.width = width;
    this->info.height = height;
    this->info.depth = depth;
    this->info.colorSpace = colorSpace;
    config();
}

void AlBitmap::config() {
    if (AlColorSpace::NONE == info.colorSpace) {
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
    info.colorSpace = AlColorSpace::NONE;
}

int AlBitmap::getWidth() { return info.width; }

int AlBitmap::getHeight() { return info.height; }

HwResult AlBitmap::resize(int width, int height, AlColorSpace colorSpace) {
    if (AlColorSpace::NONE == colorSpace) {
        Logcat::e("HWVC", "Invalid image format!");
        return Hw::FAILED;
    }
    int64_t byteSize = static_cast<uint64_t>(width * height * getImageFormatBytes(colorSpace));
    if (byteSize > this->byteSize) {
        return Hw::FAILED;
    }
    this->info.width = width;
    this->info.height = height;
    this->info.colorSpace = colorSpace;
    if (!this->pixels) {
        this->byteSize = getByteSize();
        this->pixels = new uint8_t[this->byteSize];
    }
    return Hw::SUCCESS;
}

uint8_t *AlBitmap::getPixels() { return pixels; }

uint64_t AlBitmap::getByteSize() {
    return static_cast<uint64_t>(info.width * info.height * getImageFormatBytes(info.colorSpace));
}

AlRational AlBitmap::getRotation() {
    return info.rotation;
}

void AlBitmap::dump() {
    info.dump();
}
