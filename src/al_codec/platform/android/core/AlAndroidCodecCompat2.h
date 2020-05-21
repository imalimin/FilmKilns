/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALANDROIDCODECCOMPAT2_H
#define HWVC_ANDROID_ALANDROIDCODECCOMPAT2_H

#include "AlAndroidCodecCompat.h"
#include "AlEgl.h"
#include "HwProgram.h"

al_class AlAndroidCodecCompat2 al_extend AlAndroidCodecCompat {
public:
    AlAndroidCodecCompat2(AlCodec::kID id);

    virtual ~AlAndroidCodecCompat2();

    virtual HwResult configure(HwBundle &format) override;

    virtual HwResult process(HwAbsMediaFrame **frame, HwPacket **pkt) override;

private:
    AlAndroidCodecCompat2(const AlAndroidCodecCompat2 &o) : AlAndroidCodecCompat(AlCodec::kID::NONE) {};

    bool createProgram();

private:
    bool encodeMode = true;
    AlEgl *egl = nullptr;
    AlAbsGLProgram *program = nullptr;
};


#endif //HWVC_ANDROID_ALANDROIDCODECCOMPAT2_H
