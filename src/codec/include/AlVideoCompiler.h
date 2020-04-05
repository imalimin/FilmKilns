/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_HWVIDEOOUTPUT_H
#define HWVC_ANDROID_HWVIDEOOUTPUT_H

#include "Unit.h"
#include "HwAbsVideoEncoder.h"
#include "HwVideoFrame.h"
#include "HwAudioFrame.h"
#include "AlSize.h"
#include <atomic>
#include <list>
#include <vector>

al_class_ex(AlVideoCompiler, Unit) {
public:
    AlVideoCompiler(string alias);

    virtual ~AlVideoCompiler();

    bool onCreate(AlMessage *msg) override;

    bool onDestroy(AlMessage *msg) override;

    void setRecordListener(function<void(int64_t)> listener);

private:
    bool _onSetOutPath(AlMessage *msg);

    bool _onSetSize(AlMessage *msg);

    bool _onFormat(AlMessage *msg);

    /**
     * Response read pixels message.
     */
    bool _onResponsePixels(AlMessage *msg);

    bool _onSamples(AlMessage *msg);

    bool _onWrite(AlMessage *msg);

    bool _onStart(AlMessage *msg);

    bool _onPause(AlMessage *msg);

    bool _onBackward(AlMessage *msg);

    void write(HwBuffer *buf, int64_t tsInNs);

    int64_t getRecordTimeInUs();

    void _initialize();

private:
    AlSize size;
    std::string path;
    HwSampleFormat aFormat;
    HwAbsVideoEncoder *encoder = nullptr;
    HwVideoFrame *videoFrame = nullptr;
    HwAudioFrame *audioFrame = nullptr;
    std::atomic_bool recording;
    // last timestamp in ns.
    int64_t lastTsInNs = -1, lastATsInNs = -1;
    // frame timestamp in ns.
    int64_t vTimestamp = -1, aTimestamp = -1;
    int64_t offsetOfDuration = 0;
    int count = 0;
    /**
     * Listeners
     */
    function<void(int64_t)> recordListener = nullptr;
    bool initialized = false;
};


#endif //HWVC_ANDROID_HWVIDEOOUTPUT_H
