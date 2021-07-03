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

FK_CLASS FkBitmap FK_EXTEND FkObject {
public:
    static std::shared_ptr<FkBitmap> from(std::string file);

    static FkResult write(std::string file, uint8_t *data, size_t size, int width, int height);

private:
    FkBitmap(const FkBitmap &o);

public:
    FkBitmap();

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
    Object *bmp = nullptr;
    uint8_t *pixels = nullptr;//用于存储图片像素的buffer
    uint64_t byteSize = 0;
    FkSize size;
//    AlBitmapInfo info;
};

#endif //FK_IMAGE_FKBITMAP_H