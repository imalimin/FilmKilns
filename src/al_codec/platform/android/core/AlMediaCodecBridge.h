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
    al_class(Info) {
    public:
        Info();

        ~Info();

    public:
        int offset = 0;
        int size = 0;
        long presentationTimeUs = 0;
        int flags = 0;
    };

public:
    AlMediaCodecBridge(std::string mime);

    ~AlMediaCodecBridge();

    HwResult configure(int w, int h, int bitrate, int format, int iFrameInterval, int fps);

    HwResult start();

    HwResult stop();

    HwResult flush();

    void release();

    int dequeueInputBuffer(long timeoutUs);

    HwResult queueInputBuffer(int index, int offset, int size, long presentationTimeUs, int flags);

    AlBuffer *getInputBuffer(int index);

    int dequeueOutputBuffer(AlMediaCodecBridge::Info &info, long timeoutUs);

    AlBuffer *getOutputBuffer(int index);

    HwResult releaseOutputBuffer(int index, bool render);

    AlBuffer *getOutputFormatBuffer(std::string name);

    int getOutputFormatInteger(std::string name);

private:
    jobject jHandler = nullptr;

private:
    static const JMethodDescription midInit;
    static const JMethodDescription midConfigure;
    static const JMethodDescription midStart;
    static const JMethodDescription midStop;
    static const JMethodDescription midFlush;
    static const JMethodDescription midRelease;
    static const JMethodDescription midDeqInput;
    static const JMethodDescription midQueInput;
    static const JMethodDescription midGetInput;
    static const JMethodDescription midDeqOutput;
    static const JMethodDescription midGetOutput;
    static const JMethodDescription midReleaseOutput;
    static const JMethodDescription midGetOutFmtBuf;
    static const JMethodDescription midGetOutFmtInt;
};


#endif //HWVC_ANDROID_ALMEDIACODECBRIDGE_H
