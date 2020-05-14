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

al_class AlWebPDecoder al_extend AlAbsDecoder {
public:
    AlWebPDecoder(std::string path);

    AlBitmapInfo getInfo() override;

    HwResult process(AlBuffer **buf, AlBitmapInfo *info) override;

private:
    AlWebPDecoder(const AlWebPDecoder &e) : AlAbsDecoder() {}

    bool _readFile(AlBuffer **buf);

private:
    std::string path;
};


#endif //HWVC_ANDROID_ALWEBPDECODER_H
