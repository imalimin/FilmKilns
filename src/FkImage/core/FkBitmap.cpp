/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkBitmap.h"

std::shared_ptr<FkBitmap> FkBitmap::from(AlBitmap *bmp) {
    auto target = std::shared_ptr<FkBitmap>();
    target->pixels = bmp->getPixels();
    target->byteSize = bmp->getByteSize();
    target->size.set(bmp->getWidth(), bmp->getHeight());
    return target;
}

FkBitmap::FkBitmap() : FkObject(), size(0, 0) {
    FK_MARK_SUPER
}

FkBitmap::FkBitmap(const FkBitmap &o) : FkObject(o) {
    FK_MARK_SUPER
}

FkBitmap::~FkBitmap() {

}

int FkBitmap::getWidth() {
    return size.getWidth();
}

int FkBitmap::getHeight() {
    return size.getHeight();
}

FkResult FkBitmap::resize(int width, int height, FkColor::kFormat fmt) {
    return FK_FAIL;
}

uint8_t *FkBitmap::getPixels() {
    return pixels;

}

uint64_t FkBitmap::getByteSize() {
    return byteSize;
}
