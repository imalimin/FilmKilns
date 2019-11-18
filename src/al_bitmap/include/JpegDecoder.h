/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */
#ifndef HARDWAREVIDEOCODEC_JPEGDECODER_H
#define HARDWAREVIDEOCODEC_JPEGDECODER_H

#include <string>
#include "AlAbsDecoder.h"
#include "FileUtils.h"
#include "jpeglib.h"
#include "turbojpeg.h"


class JpegDecoder : public AlAbsDecoder {
public:
    JpegDecoder(std::string path);

    JpegDecoder(JpegDecoder &e) : AlAbsDecoder() {}

    virtual ~JpegDecoder();

    AlBitmapInfo getInfo() override;

    HwResult process() override;

private:
    std::string path;
    tjhandle handle;
};


#endif //HARDWAREVIDEOCODEC_JPEGDECODER_H
