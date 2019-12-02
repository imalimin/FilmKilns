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

HwResult
HwBitmapFactory::save(int32_t w, int32_t h, AlBuffer *buf, int32_t quality, std::string path) {
    if (nullptr == buf || w <= 0 || h <= 0 || path.empty()) return Hw::FAILED;

    FILE *file = fopen(path.c_str(), "wb");
    if (file == nullptr) {
        return Hw::FAILED;
    }

    //1.创建jpeg压缩对象
    struct jpeg_compress_struct jcs;

    //错误回调
    struct jpeg_error_mgr error;
    jcs.err = jpeg_std_error(&error);

    //2.创建压缩对象
    jpeg_create_compress(&jcs);

    //3.指定存储文件
    jpeg_stdio_dest(&jcs, file);

    //4.压缩参数配置
    jcs.image_width = w;
    jcs.image_height = h;

    //yuv
    jcs.input_components = 4;
    jcs.in_color_space = JCS_EXT_RGBA;
    jpeg_set_defaults(&jcs);

    //开启哈夫曼
    jcs.arith_code = FALSE;
    jcs.optimize_coding = TRUE;
    jpeg_set_quality(&jcs, quality, TRUE);

    //5.开始压缩
    jpeg_start_compress(&jcs, TRUE);

    int row_stride = w << 2;

    JSAMPROW row[1];
    while (jcs.next_scanline < jcs.image_height) {
        uint8_t *pixels = buf->data() + jcs.next_scanline * row_stride;
        row[0] = pixels;
        jpeg_write_scanlines(&jcs, row, 1);
    }

    //6.结束压缩，释放资源
    jpeg_finish_compress(&jcs);
    fclose(file);
    jpeg_destroy_compress(&jcs);
    return Hw::SUCCESS;
}
