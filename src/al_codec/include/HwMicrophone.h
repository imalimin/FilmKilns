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
#include "HwAudioFrame.h"
#include "HwSampleFormat.h"

class HwMicrophone : public Unit {
public:
    HwMicrophone(string alias);

    virtual ~HwMicrophone();

    bool onCreate(AlMessage *msg) override;

    bool onDestroy(AlMessage *msg) override;

private:
    bool _onFormat(AlMessage *msg);

    bool _onLoop(AlMessage *msg);

    bool _onStart(AlMessage *msg);

    bool _onPause(AlMessage *msg);

    void loop();

    void send(HwBuffer *buf);

private:
    HwSampleFormat format;
    HwAudioRecorder *recorder = nullptr;
    HwAudioFrame *frame = nullptr;
    bool looping = false;
};


#endif //HWVC_ANDROID_HWMICPHONE_H
