/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/
#ifndef HARDWAREVIDEOCODEC_VIDEO_H
#define HARDWAREVIDEOCODEC_VIDEO_H

#include "HwStreamMedia.h"
#include "TextureAllocator.h"
#include "../include/AsynVideoDecoder.h"
#include "HwYV122RGBAFilter.h"
#include "HwAbsTexture.h"
#include "EventPipeline.h"
#include "Egl.h"
#include "HwAbsMediaFrame.h"
#include "HwAudioFrame.h"
#include "SimpleLock.h"
#include "NativeWindow.h"

class HwVideoInput : public HwStreamMedia {
public:
    HwVideoInput(string alias);

    virtual ~HwVideoInput();

    bool onDestroy(Message *msg) override;

    bool eventPrepare(Message *msg);

    bool eventStart(Message *msg) override;

    bool eventPause(Message *msg) override;

    bool eventSeek(Message *msg) override;

    bool eventStop(Message *msg) override;

    bool eventSetSource(Message *msg) override;

    bool eventLoop(Message *msg);

    void setPlayListener(function<void(int64_t, int64_t)> listener);

private:

    void loop();

    void checkEnv(int32_t w, int32_t h);

    HwResult grab();

    void playAudioFrame(HwAudioFrame *frame);

    void processPlayListener(int64_t us);

    void bindTex(HwVideoFrame *frame);

    bool invalidate(HwAbsTexture *tex);

private:
    const int INTERVAL_PROGRESS = 1000000;
    TextureAllocator *texAllocator = nullptr;
    AbsVideoDecoder *decoder = nullptr;
    HwYV122RGBAFilter *yuvFilter = nullptr;
    HwAbsTexture *y = nullptr;
    HwAbsTexture *u = nullptr;
    HwAbsTexture *v = nullptr;
    HwAbsTexture *target = nullptr;
    PlayState playState = STOP;
    SimpleLock simpleLock;
    string path;
    int64_t lastPts = -1;
    int64_t lastShowTime = -1;
    int64_t lastPlayPts = INT64_MIN;

    /**
     * Listeners
     */
    function<void(int64_t, int64_t)> playListener = nullptr;

    /** setting */
    bool enableLoop = true;
};


#endif //HARDWAREVIDEOCODEC_VIDEO_H
