/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "AlJpegEncoder.h"
#include "jpeglib.h"
#include "turbojpeg.h"

AlJpegEncoder::AlJpegEncoder(std::string path) : AlAbsEncoder(), path(path) {

}

AlJpegEncoder::~AlJpegEncoder() {

}

HwResult AlJpegEncoder::process(AlBuffer *buf, AlBitmapInfo *info, std::string path) {
    return process(buf, info, 80, path);
}

HwResult AlJpegEncoder::process(AlBuffer *buf, AlBitmapInfo *info, int quality, std::string path) {
    if (nullptr == buf || nullptr == info || info->width <= 0 || info->width <= 0
        || path.empty()) {
        return Hw::FAILED;
    }

    FILE *file = fopen(path.c_str(), "wb");
    if (file == nullptr) {
        return Hw::FAILED;
    }

    struct jpeg_compress_struct jcs;
    ///错误回调
    struct jpeg_error_mgr error;
    jcs.err = jpeg_std_error(&error);
    jpeg_create_compress(&jcs);
    jpeg_stdio_dest(&jcs, file);

    jcs.image_width = static_cast<JDIMENSION>(info->width);
    jcs.image_height = static_cast<JDIMENSION>(info->height);

    jcs.input_components = 4;
    jcs.in_color_space = JCS_EXT_RGBA;
    jpeg_set_defaults(&jcs);

    ///开启哈夫曼
    jcs.arith_code = FALSE;
    jcs.optimize_coding = TRUE;
    jpeg_set_quality(&jcs, quality, TRUE);

    ///压缩
    jpeg_start_compress(&jcs, TRUE);

    int row_stride = info->width << 2;

    JSAMPROW row[1];
    while (jcs.next_scanline < jcs.image_height) {
        uint8_t *pixels = buf->data() + jcs.next_scanline * row_stride;
        row[0] = pixels;
        jpeg_write_scanlines(&jcs, row, 1);
    }
    jpeg_finish_compress(&jcs);
    fclose(file);
    jpeg_destroy_compress(&jcs);
    return Hw::SUCCESS;
}
