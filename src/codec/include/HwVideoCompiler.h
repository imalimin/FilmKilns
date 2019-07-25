/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_HWVIDEOOUTPUT_H
#define HWVC_ANDROID_HWVIDEOOUTPUT_H

#include "Unit.h"
#include "../include/HwAbsEncoder.h"
#include "../include/HwVideoFrame.h"
#include "../include/HwAudioFrame.h"
#include <atomic>

class HwVideoCompiler : public Unit {
public:
    HwVideoCompiler(string alias);

    virtual ~HwVideoCompiler();

    bool eventPrepare(Message *msg);

    bool eventRelease(Message *msg) override;

    /**
     * Response read pixels message.
     */
    bool eventResponsePixels(Message *msg);

    bool eventSamples(Message *msg);

    bool eventWrite(Message *msg);

    bool eventStart(Message *msg);

    bool eventPause(Message *msg);

private:
    void write(HwBuffer *buf, int64_t tsInNs);

    int getWidth();

    int getHeight();

private:
    HwAbsEncoder *encoder = nullptr;
    HwVideoFrame *videoFrame = nullptr;
    HwAudioFrame *audioFrame = nullptr;
    std::atomic_bool recording;
    // last timestamp in ns.
    int64_t lastTsInNs = -1, lastATsInNs = -1;
    // frame timestamp in ns.
    int64_t timestamp = -1, aTimestamp = -1;
    int count = 0;
};


#endif //HWVC_ANDROID_HWVIDEOOUTPUT_H
