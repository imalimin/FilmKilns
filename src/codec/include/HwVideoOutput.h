/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_HWVIDEOOUTPUT_H
#define HWVC_ANDROID_HWVIDEOOUTPUT_H

#include "Unit.h"
#include "../include/HwFFmpegEncoder.h"
#include "../include/HwVideoFrame.h"

class HwVideoOutput : public Unit {
public:
    HwVideoOutput();

    virtual ~HwVideoOutput();

    bool eventPrepare(Message *msg);

    bool eventRelease(Message *msg) override;

    bool eventWrite(Message *msg);

private:
    HwFFmpegEncoder *encoder = nullptr;
    HwVideoFrame *videoFrame = nullptr;
    int count = 0;
};


#endif //HWVC_ANDROID_HWVIDEOOUTPUT_H
