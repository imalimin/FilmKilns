/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_HWMICPHONE_H
#define HWVC_ANDROID_HWMICPHONE_H

#include "Unit.h"
#include "HwAudioRecorder.h"
#include "../include/HwAudioFrame.h"

class HwMicPhone : public Unit {
public:
    HwMicPhone();

    virtual ~HwMicPhone();

    bool eventPrepare(Message *msg);

    bool eventRelease(Message *msg) override;

    bool eventLoop(Message *msg);

private:
    void loop();

    void send(HwBuffer *buf);

private:
    HwAudioRecorder *recorder = nullptr;
    HwAudioFrame *frame = nullptr;
};


#endif //HWVC_ANDROID_HWMICPHONE_H
