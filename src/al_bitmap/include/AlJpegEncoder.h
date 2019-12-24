/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_ALJPEGENCODER_H
#define HWVC_ANDROID_ALJPEGENCODER_H

#include "AlAbsEncoder.h"

al_class_ex(AlJpegEncoder, AlAbsEncoder) {
public:
    AlJpegEncoder(std::string path);

    AlJpegEncoder(const AlJpegEncoder &e) : AlAbsEncoder() {}

    virtual ~AlJpegEncoder();

    HwResult process(AlBuffer *buf, AlBitmapInfo *info, std::string path) override;

    HwResult process(AlBuffer *buf, AlBitmapInfo *info, int quality, std::string path);

private:
    std::string path;
};


#endif //HWVC_ANDROID_ALJPEGENCODER_H
