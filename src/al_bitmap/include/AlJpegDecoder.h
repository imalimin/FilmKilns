/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */
#ifndef HARDWAREVIDEOCODEC_JPEGDECODER_H
#define HARDWAREVIDEOCODEC_JPEGDECODER_H

#include <string>
#include <csetjmp>
#include "AlAbsDecoder.h"
#include "FileUtils.h"
#include "jpeglib.h"
#include "turbojpeg.h"

extern "C" {
struct error_mgr {
    struct jpeg_error_mgr jpegErrorMgr;
    jmp_buf jmpBuf;
};

void error_exit(j_common_ptr cinfo);
};

class AlJpegDecoder : public AlAbsDecoder {
public:
    AlJpegDecoder(std::string path);

    AlJpegDecoder(const AlJpegDecoder &e) : AlAbsDecoder() {}

    virtual ~AlJpegDecoder();

    AlBitmapInfo getInfo() override;

    HwResult process(AlBuffer **buf, AlBitmapInfo *info) override;

private:
    int _getOrientation(jpeg_saved_marker_ptr make_list);

private:
    std::string path;
    tjhandle handle;
};


#endif //HARDWAREVIDEOCODEC_JPEGDECODER_H
