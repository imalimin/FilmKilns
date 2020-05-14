/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_ALWEBPENCODER_H
#define HWVC_ANDROID_ALWEBPENCODER_H

#include "AlAbsEncoder.h"
#include "encode.h"
#include "decode.h"
#include "AlBuffer.h"

al_class AlWebPEncoder al_extend AlAbsEncoder {
public:
    AlWebPEncoder(std::string path);

    virtual ~AlWebPEncoder();

    HwResult process(AlBuffer *buf, AlBitmapInfo *info, std::string path) override;

private:
    AlWebPEncoder(const AlWebPEncoder &e) : AlAbsEncoder() {}

private:
    std::string path;

};


#endif //HWVC_ANDROID_ALWEBPENCODER_H
