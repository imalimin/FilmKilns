/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkBitmap.h"
#include "AlBitmapFactory.h"

std::shared_ptr<FkBitmap> FkBitmap::from(std::string file) {
    AlBitmap *b = AlBitmapFactory::decodeFile(file);
    if (nullptr == b) {
        return nullptr;
    }
    auto target = std::make_shared<FkBitmap>();
    target->bmp = b;
    target->pixels = b->getPixels();
    target->byteSize = b->getByteSize();
    target->size.set(b->getWidth(), b->getHeight());
    return target;
}

FkResult FkBitmap::write(std::string file, uint8_t *data, size_t size, int width, int height) {
    AlBuffer *buf = AlBuffer::wrap(data, size);
    auto ret = AlBitmapFactory::save(width, height, buf, file);
    delete buf;
    return Hw::SUCCESS == ret ? FK_OK : FK_FAIL;
}

FkBitmap::FkBitmap() : FkObject(), size(0, 0) {
    FK_MARK_SUPER
}

FkBitmap::FkBitmap(const FkBitmap &o) : FkObject(o) {
    FK_MARK_SUPER
}

FkBitmap::~FkBitmap() {
    delete bmp;
    bmp = nullptr;
    size.set(0, 0);
    pixels = nullptr;
    byteSize = 0;
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
