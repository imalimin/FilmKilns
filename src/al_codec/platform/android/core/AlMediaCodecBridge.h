/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALMEDIACODECBRIDGE_H
#define HWVC_ANDROID_ALMEDIACODECBRIDGE_H

#include "Object.h"
#include "HwAbsCodec.h"
#include "AlBuffer.h"
#include "platform/android/AlJavaNativeHelper.h"

al_class(AlMediaCodecBridge) {
public:
    AlMediaCodecBridge(int32_t codecId, bool makeNalSelf = false);

    ~AlMediaCodecBridge();

    HwResult configure(int w, int h, int bitrate, int format, int iFrameInterval, int fps);

    HwBuffer *getExtraBuffer(std::string key);

    void flush();

    void release();

    bool start();

    HwResult process(AlBuffer *buf, int64_t pts);

    int32_t type();

    HwBuffer *getBuffer();

    void getBufferInfo(size_t &size, int64_t &presentationTimeUs, bool &keyFrame);

private:
    jobject jHandler = nullptr;
    int32_t codecId = AlCodec::NONE;
    bool makeNalSelf = false;
    HwBuffer *buffers[4] = {nullptr, nullptr, nullptr, nullptr};

private:
    static const JMethodDescription midInit;
    static const JMethodDescription midConfigure;
    static const JMethodDescription midGetExtraBuffer;
    static const JMethodDescription midFlush;
    static const JMethodDescription midRelease;
    static const JMethodDescription midStart;
    static const JMethodDescription midProcess;
    static const JMethodDescription midType;
    static const JMethodDescription midGetBuffer;
    static const JMethodDescription midGetBufferInfo;
};


#endif //HWVC_ANDROID_ALMEDIACODECBRIDGE_H
