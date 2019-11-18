/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include <zconf.h>
#include "../include/PngDecoder.h"
#include "../include/log.h"

#define PNG_CHECK_BYTES 8

PngDecoder::PngDecoder(std::string path) : AlAbsDecoder(), path(path) {
    handler = png_create_read_struct(PNG_LIBPNG_VER_STRING, (png_voidp) NULL, NULL, NULL);
    if (!handler) {
        release();
        LOGE("PngDecoder init failed");
    }
    infoHandler = png_create_info_struct(handler);
    if (!infoHandler) {
        release();
        LOGE("PngDecoder init failed");
    }
}

PngDecoder::~PngDecoder() {
    release();
}

static void readCallback(png_structp handler, png_bytep data, png_size_t length) {
    ImageSource *src = (ImageSource *) png_get_io_ptr(handler);

    if (src->offset + length <= src->size) {
        memcpy(data, src->data + src->offset, length);
        src->offset += length;
    } else {
        LOGE("PNG read buf failed");
        png_error(handler, "pngReaderCallback failed");
    }
}

static void fillBuffer(uint8_t *rgba, int w, int h, png_bytep *row, int channels, int color_type) {
    if (channels == 4 || color_type == PNG_COLOR_TYPE_RGB_ALPHA) {
        for (int i = 0; i < h; ++i) {
            memcpy(rgba + i * w * 4, row[i], w * 4);
        }
    } else if (channels == 3 || color_type == PNG_COLOR_TYPE_RGB) {
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                memcpy(rgba + i * w * 4 + j * 4, row[i] + j * 3, 3);
                (rgba)[i * w * 4 + j * 4 + 3] = 255;
            }
        }
    }
}

void PngDecoder::release() {
    if (infoHandler) {
        png_destroy_info_struct(handler, &infoHandler);
    }
    if (handler) {
        png_destroy_read_struct(&handler, (png_infopp) NULL, (png_infopp) NULL);
    }
}

AlBitmapInfo PngDecoder::getInfo() {
    AlBitmapInfo info;
    FILE *file = fopen(path.c_str(), "rb");
    if (nullptr == file) {
        return info;
    }
    setjmp(png_jmpbuf(handler));
    uint8_t *buf = new uint8_t[PNG_CHECK_BYTES];
    fread(buf, 1, PNG_CHECK_BYTES, file);
    int ret = png_sig_cmp(buf, (png_size_t) 0, PNG_CHECK_BYTES);
    if (0 != ret) {
        fclose(file);
        LOGE("%s not a png(%d)", path.c_str(), ret);
        return info;//不是png文件
    }
    rewind(file);
    png_init_io(handler, file);
    png_read_png(handler, infoHandler, PNG_TRANSFORM_EXPAND, 0);
    info.width = png_get_image_width(handler, infoHandler);
    info.height = png_get_image_height(handler, infoHandler);
    info.depth = png_get_bit_depth(handler, infoHandler);
    info.colorSpace = AlColorSpace::RGBA;
    fclose(file);
    this->info = info;
    return info;
}

HwResult PngDecoder::process(HwBuffer **buf, AlBitmapInfo *info) {
    if (this->info.width <= 0 || this->info.height <= 0) {
        this->info = getInfo();
        if (this->info.width <= 0 || this->info.height <= 0) {
            return Hw::FAILED;
        }
    }
    info->width = this->info.width;
    info->height = this->info.height;
    info->depth = this->info.depth;
    info->colorSpace = this->info.colorSpace;
    *buf = HwBuffer::alloc(info->width * info->height * 4);
    int color_type = png_get_color_type(handler, infoHandler);
    int channels = png_get_channels(handler, infoHandler);
    png_bytep *row = png_get_rows(handler, infoHandler);
    fillBuffer((*buf)->data(), info->width, info->height, row, channels, color_type);
    return Hw::SUCCESS;
}