/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_HWASYNFFENCODER_H
#define HWVC_ANDROID_HWASYNFFENCODER_H

#include "HwAbsEncoder.h"
#include "EventPipeline.h"
#include "HwFrameAllocator.h"
#include <queue>

class HwAsyncEncoder : public HwAbsEncoder {
public:
    HwAsyncEncoder();

    virtual ~HwAsyncEncoder();

    virtual bool prepare(string path, int width, int height) override;

    virtual HwResult write(HwAbsMediaFrame *frame) override;

    virtual bool stop() override;

    virtual void release() override;

private:
    void loop();

    void write();

private:
    const int MAX_FRAME_CACHE = 36;
    HwAbsEncoder *encoder = nullptr;
    HwFrameAllocator *hwFrameAllocator = nullptr;
    EventPipeline *pipeline = nullptr;
    queue<HwAbsMediaFrame *> cache;
    SimpleLock simpleLock;
    SimpleLock writeBlock;
    bool looping = false;
};


#endif //HWVC_ANDROID_HWASYNFFENCODER_H
