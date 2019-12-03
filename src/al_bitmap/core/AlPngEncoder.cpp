/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "AlPngEncoder.h"
#include "../prebuild/static/libpng/include/png.h"

AlPngEncoder::AlPngEncoder(std::string path) : AlAbsEncoder(), path(path) {

}

AlPngEncoder::~AlPngEncoder() {

}

HwResult AlPngEncoder::process(AlBuffer *buf, AlBitmapInfo *info, std::string path) {
    if (nullptr == buf || nullptr == info || info->width <= 0 || info->width <= 0
        || path.empty()) {
        return Hw::FAILED;
    }

    FILE *fp = fopen(path.c_str(), "wb");
    if (!fp) {
        return Hw::FAILED;
    }
    png_byte **rows = nullptr;
    rows = (png_byte **) malloc(info->width * sizeof rows[0]);
    if (!rows) {
        fclose(fp);
        return Hw::FAILED;
    }
    for (int i = 0; i < info->height; i++)
        rows[i] = ((png_byte *) buf->data()) + i * info->width * 4;
    png_infop inf = nullptr;
    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) {
        free(rows);
        fclose(fp);
    }
    inf = png_create_info_struct(png);
    if (!inf) {
        free(rows);
        fclose(fp);
    }
    if (setjmp(png_jmpbuf(png))) {
        free(rows);
        fclose(fp);
    }
    png_init_io(png, fp);
    png_set_IHDR(png, inf, info->width, info->height, 8,
                 PNG_COLOR_TYPE_RGBA,
                 PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT
    );
    png_write_info(png, inf);

    png_write_image(png, rows);
    png_write_end(png, NULL);
    png_destroy_write_struct(&png, &inf);
    free(rows);
    fclose(fp);
    return Hw::SUCCESS;
}
