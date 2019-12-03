/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "HwBitmapFactory.h"
#include "JpegDecoder.h"
#include "PngDecoder.h"
#include "Logcat.h"
#include "AlJpegEncoder.h"
#include "AlPngEncoder.h"

HwBitmap *HwBitmapFactory::decodeFile(std::string file) {
    AlBitmapInfo info;
    AlBuffer *buf = nullptr;
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
    delete buf;
    return bitmap;
}

HwBitmap *HwBitmapFactory::decodeFile(std::string file, HwBitmap *recycleBitmap) {
    return nullptr;
}

HwBitmap *HwBitmapFactory::decodeBuffer(AlBuffer *srcBuf) {
    AlBitmapInfo info;
    AlBuffer *buf = nullptr;
    HwResult ret = Hw::FAILED;
    AlAbsDecoder *decoder = new PngDecoder(srcBuf);
    info = decoder->getInfo();
    if (!info.isNull()) { // Png format
        ret = decoder->process(&buf, &info);//先尝试以png进行解码
    }
    delete decoder;
    if (Hw::SUCCESS != ret || info.isNull()) {
        Logcat::i("HWVC", "HwBitmapFactory decodeFile buffer failed");
        return nullptr;
    }
    HwBitmap *bitmap = HwBitmap::create(info);
    memcpy(bitmap->getPixels(), buf->data(), static_cast<size_t>(bitmap->getByteSize()));
    bitmap->dump();
    delete buf;
    return bitmap;
}

HwResult HwBitmapFactory::save(int32_t w, int32_t h, AlBuffer *buf, std::string path) {
    return save(w, h, buf, 80, path);
}

HwResult HwBitmapFactory::save(int32_t w, int32_t h, AlBuffer *buf,
                               int32_t quality, std::string path) {
    if (path.empty()) return Hw::FAILED;
    AlBitmapInfo info;
    info.width = w;
    info.height = h;
    bool png = 'p' == path.data()[path.size() - 3]
               && 'n' == path.data()[path.size() - 2]
               && 'g' == path.data()[path.size() - 1];
    AlAbsEncoder *encoder = nullptr;
    if (png) {
        encoder = new AlPngEncoder(path);
    } else {
        encoder = new AlJpegEncoder(path);
    }
    HwResult ret = encoder->process(buf, &info, path);
    delete encoder;
    return ret;
}
