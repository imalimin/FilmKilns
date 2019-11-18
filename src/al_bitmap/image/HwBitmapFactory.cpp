/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "../include/HwBitmapFactory.h"
#include "../include/JpegDecoder.h"
#include "../include/PngDecoder.h"
#include "../include/Logcat.h"

HwBitmap *HwBitmapFactory::decodeFile(std::string file) {
    AlBitmapInfo info;
    HwBuffer *buf = nullptr;
    HwResult ret = Hw::FAILED;
    AlAbsDecoder *decoder = new PngDecoder(file);
    info = decoder->getInfo();
    if (!info.isNull()) { // Png format
        ret = decoder->process(&buf, &info);//先尝试以png进行解码
    }
    delete decoder;
    if (Hw::SUCCESS != ret) {//解码失败则使用jpeg解码
        decoder = new JpegDecoder(file);
        ret = decoder->process(&buf, &info);
        delete decoder;
    }
    if (Hw::SUCCESS != ret || info.isNull()) {
        Logcat::i("HWVC", "HwBitmapFactory decodeFile %s failed", file.c_str());
        return nullptr;
    }
    HwBitmap *bitmap = HwBitmap::create(info);
    memcpy(bitmap->getPixels(), buf->data(), static_cast<size_t>(bitmap->getByteSize()));
    bitmap->dump();
    delete buf;//这里重复申请了一次内存，待优化
    return bitmap;
}

HwBitmap *HwBitmapFactory::decodeFile(std::string file, HwBitmap *recycleBitmap) {
    return nullptr;
}

HwBitmap *HwBitmapFactory::decodeBuffer(HwBuffer *buf) {
    return nullptr;
}
