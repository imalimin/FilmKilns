/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "JpegDecoder.h"
#include "jpeglib.h"
#include <csetjmp>

JpegDecoder::JpegDecoder(std::string path) : AlAbsDecoder(), path(path) {
    handle = tjInitDecompress();
}

JpegDecoder::~JpegDecoder() {
    tjDestroy(handle);
}

AlBitmapInfo JpegDecoder::getInfo() {
    AlBitmapInfo info;
    FILE *file = fopen(path.c_str(), "rb");
    if (nullptr == file) {
        return info;
    }
    jpeg_decompress_struct cinfo;
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, file);
    // Decode icc profile
    jpeg_save_markers(&cinfo, JPEG_APP0 + 2, 0xFFFF);
    jpeg_read_header(&cinfo, true);

    info.width = cinfo.image_width;
    info.height = cinfo.image_height;
    info.depth = static_cast<uint32_t>(cinfo.num_components);
    if (J_COLOR_SPACE::JCS_RGB == cinfo.jpeg_color_space) {
        info.colorSpace = AlBitmapInfo::ColorSpace::RGB;
    }
    jpeg_destroy_decompress(&cinfo);
    fclose(file);
    return info;
}

HwResult JpegDecoder::process() {
//    uint8_t *buffer = nullptr;
//    unsigned long length = readFile(path, &buffer);
//    if (0 == length) {
//        return 0;
//    }
//
//    int subsample, colorspace;
//    int flags = 0;
//    int fmt = TJPF_RGBA;
//    int channels = 4;
//    tjDecompressHeader3(handle, buffer, length, width, height, &subsample, &colorspace);
//
//    flags |= 0;
//    *rgb = new uint8_t[(*width) * (*height) * channels];
//    tjDecompress2(handle, buffer, length, *rgb, *width, 0, *height, fmt, flags);
//    delete[]buffer;
    return Hw::SUCCESS;
}