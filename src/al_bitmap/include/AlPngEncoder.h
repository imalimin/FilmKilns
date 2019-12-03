/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_ALPNGENCODER_H
#define HWVC_ANDROID_ALPNGENCODER_H

#include "AlAbsEncoder.h"

al_class_ex(AlPngEncoder, AlAbsEncoder) {
public:
    AlPngEncoder(std::string path);

    AlPngEncoder(const AlPngEncoder &e) : AlAbsEncoder() {}

    virtual ~AlPngEncoder();

    HwResult process(AlBuffer *buf, AlBitmapInfo *info, std::string path) override;

public:
    std::string path;
};


#endif //HWVC_ANDROID_ALPNGENCODER_H
