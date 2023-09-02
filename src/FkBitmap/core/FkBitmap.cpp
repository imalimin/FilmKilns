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
#include "include/core/SkImage.h"
#include "include/core/SkImageEncoder.h"
#include "include/core/SkPixelRef.h"

FK_IMPL_CLASS_TYPE(FkBitmap, FkObject)

std::shared_ptr<FkBitmap> FkBitmap::from(const std::shared_ptr<SkBitmap> &bmp) {
    return std::shared_ptr<FkBitmap>(new FkBitmap(bmp));
}

std::shared_ptr<FkBitmap> FkBitmap::from(const std::string &file) {
    auto bmp =  std::shared_ptr<FkBitmap>(new FkBitmap(file));
    if (bmp->bmp == nullptr) {
        return nullptr;
    }
    return bmp;
}

FkResult FkBitmap::write(std::string file, FkImage::Format fmt, std::shared_ptr<FkBuffer> buf,
                         FkSize size, int quality) {
    sk_sp<SkPixelRef> pixelRef = sk_make_sp<SkPixelRef>(size.getWidth(),
                                                        size.getHeight(),
                                                        buf->data(),
                                                        size.getWidth() * 4);
    auto bmp = std::make_shared<SkBitmap>();
    bmp->setInfo(SkImageInfo::Make(pixelRef->width(), pixelRef->height(),kRGBA_8888_SkColorType,kOpaque_SkAlphaType), pixelRef->rowBytes());
    bmp->setPixelRef(pixelRef, 0, 0);
    SkEncodedImageFormat skFmt;
    switch (fmt) {
        case FkImage::Format::kBMP: {
            skFmt = SkEncodedImageFormat::kBMP;
            break;
        }
        case FkImage::Format::kJPEG: {
            skFmt = SkEncodedImageFormat::kJPEG;
            break;
        }
        case FkImage::Format::kPNG: {
            skFmt = SkEncodedImageFormat::kPNG;
            break;
        }
        case FkImage::Format::kWEBP: {
            skFmt = SkEncodedImageFormat::kWEBP;
            break;
        }
        default:
            skFmt = SkEncodedImageFormat::kJPEG;
    }
    auto encodedData = SkEncodeBitmap(*bmp, skFmt, quality);
    if (encodedData != nullptr) {
        FILE *f = fopen(file.c_str(), "wb");
        if (f) {
            fwrite(encodedData->data(), 1, encodedData->size(), f);
            fclose(f);
            return FK_OK;
        }
        return FK_IO_FAIL;
    }
    return FK_FAIL;
}

FkBitmap::FkBitmap(const std::string &file) : FkObject() {

    sk_sp<SkData> data = SkData::MakeFromFileName(file.c_str());
    std::unique_ptr<SkCodec> codec = SkCodec::MakeFromData(data);
    if (!codec) {
        FkLogI(FK_DEF_TAG, "Can not find codec for %s", file.c_str());
        return;
    }
    SkImageInfo info = codec->getInfo().makeColorType(kN32_SkColorType)
            .makeAlphaType(SkAlphaType::kPremul_SkAlphaType);
    bmp = std::make_shared<SkBitmap>();
    if (!bmp->tryAllocPixels(info)) {
        FkLogI(FK_DEF_TAG, "Can not alloc pixels mem.");
        return;
    }
    if (SkCodec::kSuccess != codec->getPixels(info, bmp->getPixels(), bmp->rowBytes())) {
        FkLogI(FK_DEF_TAG, "Can not read pixels.");
    }
    _setEncodedOrigin(codec->getOrigin());
}

FkBitmap::FkBitmap(const std::shared_ptr<SkBitmap> &bmp) {
    this->bmp = bmp;
}

FkBitmap::FkBitmap(const FkBitmap &o) : FkObject(o) {

}

void FkBitmap::_setEncodedOrigin(int32_t skEncodedOrigin) {
    encodedOrigin = static_cast<FkEncodedOrigin>(skEncodedOrigin);
    FkLogI(FK_DEF_TAG, "orientation: %d", encodedOrigin);
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

FkEncodedOrigin FkBitmap::getEncodedOrigin() {
    return encodedOrigin;
}

bool FkBitmap::isSwappedWH() {
    return encodedOrigin >= FkEncodedOrigin::kLeftTop && encodedOrigin <= FkEncodedOrigin::kLeftBottom;
}
