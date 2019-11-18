/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "JpegDecoder.h"
#include "jpeglib.h"
#include "Logcat.h"
#include <csetjmp>

void error_exit(j_common_ptr cinfo) {
/* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
    error_mgr *errorMgr = (error_mgr *) cinfo->err;

/* Always display the message. */
/* We could postpone this until after returning, if we chose. */
    (*cinfo->err->output_message)(cinfo);

/* Return control to the setjmp point */
    longjmp(errorMgr->jmpBuf, 1);
}

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
    error_mgr errorMgr;
    cinfo.err = jpeg_std_error(&errorMgr.jpegErrorMgr);
    cinfo.image_width = 0;
    cinfo.image_height = 0;
    errorMgr.jpegErrorMgr.error_exit = error_exit;
    if (setjmp(errorMgr.jmpBuf)) {
        jpeg_destroy_decompress(&cinfo);
        fclose(file);
        return info;
    }

    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, file);
    // Decode icc profile
    jpeg_save_markers(&cinfo, JPEG_APP0 + 2, 0xFFFF);
    jpeg_read_header(&cinfo, true);

    info.width = cinfo.image_width;
    info.height = cinfo.image_height;
    info.depth = 8;
    info.colorSpace = AlColorSpace::RGBA;
    jpeg_destroy_decompress(&cinfo);
    fclose(file);
    return info;
}

HwResult JpegDecoder::process(HwBuffer **buf, AlBitmapInfo *info) {
    uint8_t *buffer = nullptr;
    unsigned long length = readFile(path, &buffer);
    if (0 == length) {
        return 0;
    }

    int subsample, colorspace;
    int flags = 0;
    int fmt = TJPF_RGBA;
    int channels = 4;
    tjDecompressHeader3(handle, buffer, length, &info->width, &info->height, &subsample, &colorspace);

    flags |= 0;
    *buf = HwBuffer::alloc(info->width * info->height * channels);
    tjDecompress2(handle, buffer, length, (*buf)->data(), info->width, 0, info->height, fmt, flags);
    delete[]buffer;
    info->depth = 8;
    info->colorSpace = AlColorSpace::RGBA;
    return Hw::SUCCESS;
}