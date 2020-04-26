/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALMEDIACODECBRIDGE_H
#define HWVC_ANDROID_ALMEDIACODECBRIDGE_H

#include "Object.h"
#include "jni.h"

al_class(AlMediaCodecBridge) {
public:
    AlMediaCodecBridge();

    ~AlMediaCodecBridge();

private:
    jobject jHandler = nullptr;
};


#endif //HWVC_ANDROID_ALMEDIACODECBRIDGE_H
