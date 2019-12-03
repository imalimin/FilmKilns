/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_ABSENCODER_H
#define HWVC_ANDROID_ABSENCODER_H

#include "Object.h"
#include "AlBuffer.h"
#include "HwResult.h"
#include "AlBitmapInfo.h"

al_interface(AlAbsEncoder) {
public:
    AlAbsEncoder() : Object() {}

    AlAbsEncoder(AlAbsEncoder &e) : Object() {}

    virtual ~AlAbsEncoder() {};

    virtual HwResult process(AlBuffer *buf, AlBitmapInfo *info, std::string path) = 0;
};


#endif //HWVC_ANDROID_ABSENCODER_H
