/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include <zconf.h>
#include "AlPngDecoder.h"
#include "log.h"

#define PNG_CHECK_BYTES 8

AlPngDecoder::AlPngDecoder(std::string path) : AlAbsDecoder(), path(path) {
    handler = png_create_read_struct(PNG_LIBPNG_VER_STRING, (png_voidp) NULL, NULL, NULL);
    if (!handler) {
        release();
        LOGE("AlPngDecoder init failed");
    }
    infoHandler = png_create_info_struct(handler);
    if (!infoHandler) {
        release();
        LOGE("AlPngDecoder init failed");
    }
}

AlPngDecoder::AlPngDecoder(AlBuffer *buf) : AlAbsDecoder(), buf(buf) {
    handler = png_create_read_struct(PNG_LIBPNG_VER_STRING, (png_voidp) NULL, NULL, NULL);
    if (!handler) {
        release();
        LOGE("AlPngDecoder init failed");
    }
    infoHandler = png_create_info_struct(handler);
    if (!infoHandler) {
        release();
        LOGE("AlPngDecoder init failed");
    }
}

AlPngDecoder::~AlPngDecoder() {
    release();
}

static void fillBuffer(uint8_t *rgba, int w, int h, png_bytep *row,
                       int channels, int color_type, int offset) {
    if (channels == 4 || color_type == PNG_COLOR_TYPE_RGB_ALPHA) {
        if (1 == offset) {
            for (int i = 0; i < h; ++i) {
                memcpy(rgba + i * w * 4, row[i], w * 4);
            }
        } else {
            for (int i = 0; i < h; ++i) {
                for (int j = 0; j < w; ++j) {
                    rgba[i * w * 4 + j * 4] = row[i][j * 4 * offset];
                    rgba[i * w * 4 + j * 4 + 1] = row[i][j * 4 * offset + 1 * offset];
                    rgba[i * w * 4 + j * 4 + 2] = row[i][j * 4 * offset + 2 * offset];
                    rgba[i * w * 4 + j * 4 + 3] = row[i][j * 4 * offset + 3 * offset];;
                }
            }
        }
    } else if (channels == 3 || color_type == PNG_COLOR_TYPE_RGB) {
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                rgba[i * w * 4 + j * 4] = row[i][j * 3 * offset];
                rgba[i * w * 4 + j * 4 + 1] = row[i][j * 3 * offset + 1 * offset];
                rgba[i * w * 4 + j * 4 + 2] = row[i][j * 3 * offset + 2 * offset];
                rgba[i * w * 4 + j * 4 + 3] = 255;
            }
        }
    }
}

void AlPngDecoder::release() {
    if (infoHandler) {
        png_destroy_info_struct(handler, &infoHandler);
    }
    if (handler) {
        png_destroy_read_struct(&handler, (png_infopp) NULL, (png_infopp) NULL);
    }
}

AlBitmapInfo AlPngDecoder::getInfo() {
    AlBitmapInfo info;
    if (buf) {
        getInfo(info, buf);
    } else {
        getInfo(info, path);
    }
    this->info = info;
    return info;
}

HwResult AlPngDecoder::process(AlBuffer **buf, AlBitmapInfo *info) {
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
    *buf = AlBuffer::alloc(info->width * info->height * 4);
    int color_type = png_get_color_type(handler, infoHandler);
    int channels = png_get_channels(handler, infoHandler);
    png_bytep *row = png_get_rows(handler, infoHandler);
    fillBuffer((*buf)->data(), info->width, info->height, row, channels, color_type,
               this->info.depth / 8);
    return Hw::SUCCESS;
}

void AlPngDecoder::getInfo(AlBitmapInfo &info, std::string &path) {
    FILE *file = fopen(path.c_str(), "rb");
    if (nullptr == file) {
        return;
    }
    if (setjmp(png_jmpbuf(handler))) {
        return;
    }
    uint8_t *buf = new uint8_t[PNG_CHECK_BYTES];
    fread(buf, 1, PNG_CHECK_BYTES, file);
    int ret = png_sig_cmp(buf, (png_size_t) 0, PNG_CHECK_BYTES);
    if (0 != ret) {
        fclose(file);
        Logcat::i("AlPngDecoder", "%s(%d): Invalid png file", __FUNCTION__, __LINE__);
        return;//不是png文件
    }
    rewind(file);
    png_init_io(handler, file);
    png_read_png(handler, infoHandler, PNG_TRANSFORM_EXPAND, 0);
    info.width = png_get_image_width(handler, infoHandler);
    info.height = png_get_image_height(handler, infoHandler);
    info.depth = png_get_bit_depth(handler, infoHandler);
    info.colorSpace = AlColorSpace::RGBA;
    fclose(file);
}

static void readFunc(png_structp handler, png_bytep data, png_size_t length) {
    AlPngDecoder::AlImageSource *src = (AlPngDecoder::AlImageSource *) png_get_io_ptr(handler);
    if (src->offset + length <= src->size) {
        memcpy(data, src->data + src->offset, length);
        src->offset += length;
    } else {
        LOGE("PNG read buf failed");
        png_error(handler, "pngReaderCallback failed");
    }
}

void AlPngDecoder::getInfo(AlBitmapInfo &info, AlBuffer *buf) {
    if (setjmp(png_jmpbuf(handler))) {
        return;
    }
    AlImageSource src;
    src.data = buf->data();
    src.size = buf->size();
    src.offset = 0;
    int ret = png_sig_cmp(src.data, (png_size_t) 0, PNG_CHECK_BYTES);
    if (0 != ret) {
        Logcat::i("AlPngDecoder", "%s(%d): Invalid png file", __FUNCTION__, __LINE__);
        return;//不是png文件
    }
    png_set_read_fn(handler, &src, readFunc);
    png_read_png(handler, infoHandler, PNG_TRANSFORM_EXPAND, 0);
    info.width = png_get_image_width(handler, infoHandler);
    info.height = png_get_image_height(handler, infoHandler);
    info.depth = png_get_bit_depth(handler, infoHandler);
    info.colorSpace = AlColorSpace::RGBA;
}
