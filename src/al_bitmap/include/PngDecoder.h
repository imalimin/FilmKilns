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

class PngDecoder : public AlAbsDecoder {
public:
    typedef struct {
        uint8_t *data;
        int size;
        int offset;
    } AlImageSource;
public:
    PngDecoder(std::string path);

    PngDecoder(HwBuffer *buf);

    PngDecoder(const PngDecoder &e) : AlAbsDecoder(), path("") {};

    virtual ~PngDecoder();

    AlBitmapInfo getInfo() override;

    HwResult process(HwBuffer **buf, AlBitmapInfo *info) override;

private:
    std::string path;
    HwBuffer *buf = nullptr;
    png_structp handler;
    png_infop infoHandler;
    AlBitmapInfo info;

    void release();

    void getInfo(AlBitmapInfo &info, std::string &path);

    void getInfo(AlBitmapInfo &info, HwBuffer *buf);
};


#endif //HARDWAREVIDEOCODEC_PNGDECODER_H
