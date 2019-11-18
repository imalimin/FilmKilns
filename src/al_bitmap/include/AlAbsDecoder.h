/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_ABSDECODER_H
#define HWVC_ANDROID_ABSDECODER_H

#include "Object.h"
#include "HwResult.h"
#include "AlBitmapInfo.h"

al_interface(AlAbsDecoder) {
public:
    AlAbsDecoder() : Object() {}

    AlAbsDecoder(AlAbsDecoder &e) : Object() {}

    virtual ~AlAbsDecoder() {};

    virtual AlBitmapInfo getInfo() = 0;

    virtual HwResult process(uint8_t **buf, AlBitmapInfo *info) = 0;
};


#endif //HWVC_ANDROID_ABSDECODER_H
