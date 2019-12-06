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

class HwMicrophone : public Unit {
public:
    HwMicrophone(string alias);

    virtual ~HwMicrophone();

    bool onCreate(Message *msg) override;

    bool onDestroy(Message *msg) override;

    bool eventLoop(Message *msg);

    bool eventStart(Message *msg);

    bool eventPause(Message *msg);

private:
    void loop();

    void send(HwBuffer *buf);

private:
    HwAudioRecorder *recorder = nullptr;
    HwAudioFrame *frame = nullptr;
    bool looping = false;
};


#endif //HWVC_ANDROID_HWMICPHONE_H
