/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_IMAGE_FKBITMAP_H
#define FK_IMAGE_FKBITMAP_H

#include "FkObject.h"
#include "FkColor.h"
#include "FkSize.h"
#include "FkBuffer.h"
#include "FkImage.h"
#include "include/core/SkBitmap.h"

FK_CLASS FkBitmap FK_EXTEND FkObject {
public:
    static std::shared_ptr<FkBitmap> from(std::string &file);

    static FkResult write(std::string file, FkImage::Format fmt, std::shared_ptr<FkBuffer> buf, FkSize size, int quality);

private:
    FkBitmap(const FkBitmap &o);

    FkBitmap(std::string &file);

public:
    virtual ~FkBitmap();

    int getWidth();

    int getHeight();

    /**
     * 改变HwBitmap大小和格式，用于内存复用
     */
    FkResult resize(int width, int height, FkColor::kFormat fmt);

    /**
     * 获取指向图片像素内存的指针
     */
    uint8_t *getPixels();

    /**
     * 获取图片占用的内存大小
     */
    uint64_t getByteSize();

//    AlRational getRotation();

//    void dump();

private:
    std::shared_ptr<SkBitmap> bmp = nullptr;
};

#endif //FK_IMAGE_FKBITMAP_H