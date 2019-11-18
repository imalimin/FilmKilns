/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */
#ifndef HARDWAREVIDEOCODEC_PNGDECODER_H
#define HARDWAREVIDEOCODEC_PNGDECODER_H

#include "AlAbsDecoder.h"
#include "FileUtils.h"
#include "../prebuild/static/libpng/include/png.h"

typedef struct {
    uint8_t *data;
    int size;
    int offset;
} ImageSource;

class PngDecoder : public AlAbsDecoder {
public:
    PngDecoder(std::string path);

    PngDecoder(const PngDecoder &e) : AlAbsDecoder() {};

    virtual ~PngDecoder();

    AlBitmapInfo getInfo() override;

    HwResult process(HwBuffer **buf, AlBitmapInfo *info) override;

private:
    std::string path;
    png_structp handler;
    png_infop infoHandler;
    AlBitmapInfo info;

    void release();
};


#endif //HARDWAREVIDEOCODEC_PNGDECODER_H
