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

private:
    jobject jHandler = nullptr;
    int32_t codecId = AlCodec::NONE;
    bool makeNalSelf = false;
    HwBuffer *buffers[4] = {nullptr, nullptr, nullptr, nullptr};

private:
    static JMethodDescription midInit = {
            "com/lmy/hwvcnative/core/AlMediaCodec",
            "<init>", "(IZZ)V"};
    static JMethodDescription midConfigure = {
            "com/lmy/hwvcnative/core/AlMediaCodec",
            "configure", "(IIIIII)Z"};
    static JMethodDescription midGetExtraBuffer = {
            "com/lmy/hwvcnative/core/AlMediaCodec",
            "getExtraBuffer", "(Ljava/lang/String;)Ljava/nio/ByteBuffer;"};
    static JMethodDescription midFlush = {
            "com/lmy/hwvcnative/core/AlMediaCodec",
            "flush", "()V"};
    static JMethodDescription midRelease = {
            "com/lmy/hwvcnative/core/AlMediaCodec",
            "release", "()V"};
    static JMethodDescription midStart = {
            "com/lmy/hwvcnative/core/AlMediaCodec",
            "start", "()Z"};
    static JMethodDescription midProcess = {
            "com/lmy/hwvcnative/core/AlMediaCodec",
            "process", "([BJ)I"};
    static JMethodDescription midType = {
            "com/lmy/hwvcnative/core/AlMediaCodec",
            "type", "()I"};
    static JMethodDescription midGetBuffer = {
            "com/lmy/hwvcnative/core/AlMediaCodec",
            "getBuffer", "()[B"};
    static JMethodDescription midGetBufferInfo = {
            "com/lmy/hwvcnative/core/AlMediaCodec",
            "getBufferInfo", "()[J"};
};


#endif //HWVC_ANDROID_ALMEDIACODECBRIDGE_H
