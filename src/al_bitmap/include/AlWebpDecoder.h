/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_ALWEBPDECODER_H
#define HWVC_ANDROID_ALWEBPDECODER_H

#include "AlAbsDecoder.h"
#include "encode.h"
#include "decode.h"
#include "AlBuffer.h"

class AlWebpDecoder : public AlAbsDecoder {
public:
    AlWebpDecoder(std::string path);

    AlBitmapInfo getInfo() override;

    HwResult process(AlBuffer **buf, AlBitmapInfo *info) override;

private:
    AlWebpDecoder(const AlWebpDecoder &e) : AlAbsDecoder() {}

    bool _readFile(AlBuffer **buf);

private:
    std::string path;
    WebPConfig config;
};


#endif //HWVC_ANDROID_ALWEBPDECODER_H
