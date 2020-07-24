/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALANDROIDCODECDECCOMPAT_H
#define HWVC_ANDROID_ALANDROIDCODECDECCOMPAT_H

#include "AlAndroidCodecCompat.h"

al_class AlAndroidCodecDecCompat al_extend AlAndroidCodecCompat {
public:
    AlAndroidCodecDecCompat(AlCodec::kID id);

    virtual ~AlAndroidCodecDecCompat();

    virtual HwResult configure(AlBundle &format) override ;
};


#endif //HWVC_ANDROID_ALANDROIDCODECDECCOMPAT_H
