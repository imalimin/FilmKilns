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
#include "HwWindow.h"
#include "platform/android/AlJNIEnv.h"
#include <android/native_window_jni.h>

al_class AlMediaCodecBridge al_extend Object {
public:
    al_class Info al_extend Object {
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
    AlMediaCodecBridge(const std::string &mime);

    ~AlMediaCodecBridge();

    HwResult configure(int w, int h, int bitrate, int format, int iFrameInterval, int fps, int flags);

    HwWindow *createInputSurface();

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
    static const AlJNIObject::Method midInit;
    static const AlJNIObject::Method midConfigure;
    static const AlJNIObject::Method midInputSurface;
    static const AlJNIObject::Method midStart;
    static const AlJNIObject::Method midStop;
    static const AlJNIObject::Method midFlush;
    static const AlJNIObject::Method midRelease;
    static const AlJNIObject::Method midDeqInput;
    static const AlJNIObject::Method midQueInput;
    static const AlJNIObject::Method midGetInput;
    static const AlJNIObject::Method midDeqOutput;
    static const AlJNIObject::Method midGetOutput;
    static const AlJNIObject::Method midReleaseOutput;
    static const AlJNIObject::Method midGetOutFmtBuf;
    static const AlJNIObject::Method midGetOutFmtInt;
};


#endif //HWVC_ANDROID_ALMEDIACODECBRIDGE_H
