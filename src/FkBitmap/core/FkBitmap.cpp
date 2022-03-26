/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkBitmap.h"
#include "include/core/SkData.h"
#include "include/core/SkRefCnt.h"
#include "include/codec/SkCodec.h"

std::shared_ptr<FkBitmap> FkBitmap::from(std::string &file) {
    return std::shared_ptr<FkBitmap>(new FkBitmap(file));
}

FkResult FkBitmap::write(std::string file, uint8_t *data, size_t size, int width, int height) {
//    AlBuffer *buf = AlBuffer::wrap(data, size);
//    auto ret = AlBitmapFactory::save(width, height, buf, file);
//    delete buf;
//    return Hw::SUCCESS == ret ? FK_OK : FK_FAIL;
    return FK_FAIL;
}

FkBitmap::FkBitmap(std::string &file) : FkObject() {
    FK_MARK_SUPER
    SkColorType colorType = kN32_SkColorType;
    bmp = std::make_shared<SkBitmap>();
    sk_sp<SkData> data = SkData::MakeFromFileName(file.c_str());
    std::unique_ptr<SkCodec> codec = SkCodec::MakeFromData(data);
    if (!codec) {
        FkLogI(FK_DEF_TAG, "Can not find codec for %s", file.c_str());
        return;
    }
    SkImageInfo info = codec->getInfo().makeColorType(colorType);
    if (!bmp->tryAllocPixels(info)) {
        FkLogI(FK_DEF_TAG, "Can not alloc pixels mem.");
        return;
    }
    if (SkCodec::kSuccess != codec->getPixels(info, bmp->getPixels(), bmp->rowBytes())) {
        FkLogI(FK_DEF_TAG, "Can not read pixels.");
    }
}

FkBitmap::FkBitmap(const FkBitmap &o) : FkObject(o) {
    FK_MARK_SUPER
}

FkBitmap::~FkBitmap() {
    bmp = nullptr;
}

int FkBitmap::getWidth() {
    return bmp->width();
}

int FkBitmap::getHeight() {
    return bmp->height();
}

FkResult FkBitmap::resize(int width, int height, FkColor::kFormat fmt) {
    return FK_FAIL;
}

uint8_t *FkBitmap::getPixels() {
    return (uint8_t *) bmp->getPixels();

}

uint64_t FkBitmap::getByteSize() {
    return bmp->computeByteSize();
}
