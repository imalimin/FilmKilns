/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_HWAUDIOCOMPILER_H
#define HWVC_ANDROID_HWAUDIOCOMPILER_H

#include "Unit.h"
#include "HwFFAudioEncoder.h"
#include "WAVRawMuxer.h"

class HwAudioCompiler : public Unit {
public:
    HwAudioCompiler(string alias);

    virtual ~HwAudioCompiler();

    bool onDestroy(AlMessage *msg) override;

    bool onCreate(AlMessage *msg) override;

    bool eventReceiveData(AlMessage *msg);

private:
    HwFFAudioEncoder *encoder = nullptr;
    WAVRawMuxer *muxer = nullptr;
};


#endif //HWVC_ANDROID_HWAUDIOCOMPILER_H
