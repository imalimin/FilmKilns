/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlMediaCodecBridge.h"

#define TAG "AlMediaCodecBridge"

const JMethodDescription AlMediaCodecBridge::midInit = {
        "com/lmy/hwvcnative/core/AlMediaCodecKt",
        "<init>", "(Ljava/lang/String;)V"};
const JMethodDescription AlMediaCodecBridge::midConfigure = {
        "com/lmy/hwvcnative/core/AlMediaCodecKt",
        "configure", "(IIIIII)I"};
const JMethodDescription AlMediaCodecBridge::midStart = {
        "com/lmy/hwvcnative/core/AlMediaCodecKt",
        "start", "()I"};
const JMethodDescription AlMediaCodecBridge::midStop = {
        "com/lmy/hwvcnative/core/AlMediaCodecKt",
        "stop", "()I"};
const JMethodDescription AlMediaCodecBridge::midFlush = {
        "com/lmy/hwvcnative/core/AlMediaCodecKt",
        "flush", "()I"};
const JMethodDescription AlMediaCodecBridge::midRelease = {
        "com/lmy/hwvcnative/core/AlMediaCodecKt",
        "release", "()V"};
const JMethodDescription AlMediaCodecBridge::midDeqInput = {
        "com/lmy/hwvcnative/core/AlMediaCodecKt",
        "dequeueInputBuffer", "(J)I"};
const JMethodDescription AlMediaCodecBridge::midQueInput = {
        "com/lmy/hwvcnative/core/AlMediaCodecKt",
        "queueInputBuffer", "(IIIJI)I"};
const JMethodDescription AlMediaCodecBridge::midGetInput = {
        "com/lmy/hwvcnative/core/AlMediaCodecKt",
        "getInputBuffer", "(I)Ljava/nio/ByteBuffer;"};
const JMethodDescription AlMediaCodecBridge::midDeqOutput = {
        "com/lmy/hwvcnative/core/AlMediaCodecKt",
        "dequeueOutputBuffer", "(J)[J"};
const JMethodDescription AlMediaCodecBridge::midGetOutput = {
        "com/lmy/hwvcnative/core/AlMediaCodecKt",
        "getOutputBuffer", "(I)Ljava/nio/ByteBuffer;"};
const JMethodDescription AlMediaCodecBridge::midReleaseOutput = {
        "com/lmy/hwvcnative/core/AlMediaCodecKt",
        "releaseOutputBuffer", "(IB)I"};
const JMethodDescription AlMediaCodecBridge::midGetOutFmtBuf = {
        "com/lmy/hwvcnative/core/AlMediaCodecKt",
        "getOutputFormatBuffer", "(Ljava/lang/String;)Ljava/nio/ByteBuffer;"};
const JMethodDescription AlMediaCodecBridge::midGetOutFmtInt = {
        "com/lmy/hwvcnative/core/AlMediaCodecKt",
        "getOutputFormatInteger", "(Ljava/lang/String;)I"};

AlMediaCodecBridge::Info::Info() {

}

AlMediaCodecBridge::Info::~Info() {

}

AlMediaCodecBridge::AlMediaCodecBridge(std::string mime) : Object() {
    JNIEnv *env = nullptr;
    if (!AlJavaNativeHelper::getInstance()->findEnv(&env)) {
        AlLogE(TAG, "failed");
        return;
    }
    jclass cls = AlJavaNativeHelper::getInstance()->registerAnClass(midInit.cls.c_str());
    if (nullptr == cls) {
        AlLogE(TAG, "failed");
        return;
    }
    auto mid = env->GetMethodID(cls, midInit.name.c_str(), midInit.sign.c_str());
    if (nullptr == mid) {
        AlLogE(TAG, "failed");
        return;
    }
    const char *str = mime.c_str();
    jstring jstr = env->NewStringUTF(str);
    jHandler = env->NewObject(cls, mid, jstr);
    env->ReleaseStringUTFChars(jstr, str);
    env->ExceptionCheck();
    env->ExceptionClear();
    if (nullptr == jHandler) {
        AlLogE(TAG, "failed");
        return;
    }
    AlJavaNativeHelper::getInstance()->registerAnObject(env,
                                                        reinterpret_cast<jlong>(this),
                                                        jHandler);
    AlLogI(TAG, "success");
}

AlMediaCodecBridge::~AlMediaCodecBridge() {
    JNIEnv *env = nullptr;
    if (AlJavaNativeHelper::getInstance()->findEnv(&env)) {
        env->DeleteLocalRef(jHandler);
        AlJavaNativeHelper::getInstance()->unregisterAnObject(env, reinterpret_cast<jlong>(this));
    }
}

HwResult AlMediaCodecBridge::configure(int w, int h,
                                       int bitrate,
                                       int format,
                                       int iFrameInterval,
                                       int fps) {
    AlLogI(TAG, "enter.");
    JNIEnv *env = nullptr;
    jmethodID mid = nullptr;
    bool ret0 = AlJavaNativeHelper::getInstance()->findEnv(&env);
    bool ret1 = AlJavaNativeHelper::getInstance()->findMethod(reinterpret_cast<jlong>(this),
                                                              midConfigure,
                                                              &mid);
    if (ret0 && ret1) {
        int ret = env->CallIntMethod(jHandler, mid, w, h, bitrate, format, iFrameInterval, fps);
        env->ExceptionCheck();
        env->ExceptionClear();
        return HwResult(ret);
    }
    AlLogE(TAG, "failed");
    return Hw::FAILED;
}

HwResult AlMediaCodecBridge::start() {
    JNIEnv *env = nullptr;
    jmethodID mid = nullptr;
    if (AlJavaNativeHelper::getInstance()->findEnv(&env) &&
        AlJavaNativeHelper::getInstance()->findMethod(reinterpret_cast<jlong>(this),
                                                      midStart,
                                                      &mid)) {
        int ret = env->CallIntMethod(jHandler, mid);
        env->ExceptionCheck();
        env->ExceptionClear();
        return HwResult(ret);
    }
    AlLogE(TAG, "failed");
    return Hw::FAILED;
}

HwResult AlMediaCodecBridge::stop() {
    JNIEnv *env = nullptr;
    jmethodID mid = nullptr;
    if (AlJavaNativeHelper::getInstance()->findEnv(&env) &&
        AlJavaNativeHelper::getInstance()->findMethod(reinterpret_cast<jlong>(this),
                                                      midStop,
                                                      &mid)) {
        int ret = env->CallIntMethod(jHandler, mid);
        env->ExceptionCheck();
        env->ExceptionClear();
        return HwResult(ret);
    }
    AlLogE(TAG, "failed");
    return Hw::FAILED;
}

HwResult AlMediaCodecBridge::flush() {
    JNIEnv *env = nullptr;
    jmethodID mid = nullptr;
    if (AlJavaNativeHelper::getInstance()->findEnv(&env) &&
        AlJavaNativeHelper::getInstance()->findMethod(reinterpret_cast<jlong>(this),
                                                      midFlush,
                                                      &mid)) {
        int ret = env->CallIntMethod(jHandler, mid);
        env->ExceptionCheck();
        env->ExceptionClear();
        return HwResult(ret);
    }
    AlLogE(TAG, "failed");
    return Hw::FAILED;
}

void AlMediaCodecBridge::release() {
    JNIEnv *env = nullptr;
    jmethodID mid = nullptr;
    if (AlJavaNativeHelper::getInstance()->findEnv(&env) &&
        AlJavaNativeHelper::getInstance()->findMethod(reinterpret_cast<jlong>(this),
                                                      midRelease,
                                                      &mid)) {
        env->CallVoidMethod(jHandler, mid);
        env->ExceptionCheck();
        env->ExceptionClear();
    }
    AlLogE(TAG, "failed");
}

int AlMediaCodecBridge::dequeueInputBuffer(long timeoutUs) {
    JNIEnv *env = nullptr;
    jmethodID mid = nullptr;
    if (AlJavaNativeHelper::getInstance()->findEnv(&env) &&
        AlJavaNativeHelper::getInstance()->findMethod(reinterpret_cast<jlong>(this),
                                                      midDeqInput,
                                                      &mid)) {
        int ret = env->CallIntMethod(jHandler, mid, timeoutUs);
        env->ExceptionCheck();
        env->ExceptionClear();
        return ret;
    }
    AlLogE(TAG, "failed");
    return -1;
}

HwResult AlMediaCodecBridge::queueInputBuffer(int index,
                                              int offset,
                                              int size,
                                              long presentationTimeUs,
                                              int flags) {
    JNIEnv *env = nullptr;
    jmethodID mid = nullptr;
    if (AlJavaNativeHelper::getInstance()->findEnv(&env) &&
        AlJavaNativeHelper::getInstance()->findMethod(reinterpret_cast<jlong>(this),
                                                      midQueInput,
                                                      &mid)) {
        int ret = env->CallIntMethod(jHandler, mid, index, offset, size, presentationTimeUs, flags);
        env->ExceptionCheck();
        env->ExceptionClear();
        return HwResult(ret);
    }
    AlLogE(TAG, "failed");
    return Hw::SUCCESS;
}

AlBuffer *AlMediaCodecBridge::getInputBuffer(int index) {
    JNIEnv *env = nullptr;
    jmethodID mid = nullptr;
    if (AlJavaNativeHelper::getInstance()->findEnv(&env) &&
        AlJavaNativeHelper::getInstance()->findMethod(reinterpret_cast<jlong>(this),
                                                      midGetInput,
                                                      &mid)) {
        jobject obj = env->CallObjectMethod(jHandler, mid, index);
        if (nullptr == obj) {
            AlLogE(TAG, "failed");
            return nullptr;
        }
        jlong capacity = env->GetDirectBufferCapacity(obj);
        void *buf = env->GetDirectBufferAddress(obj);
        env->ExceptionCheck();
        env->ExceptionClear();
        return AlBuffer::wrap(static_cast<uint8_t *>(buf), static_cast<size_t>(capacity));
    }
    AlLogE(TAG, "failed");
    return nullptr;
}

int AlMediaCodecBridge::dequeueOutputBuffer(AlMediaCodecBridge::Info &info, long timeoutUs) {
    JNIEnv *env = nullptr;
    jmethodID mid = nullptr;
    if (AlJavaNativeHelper::getInstance()->findEnv(&env) &&
        AlJavaNativeHelper::getInstance()->findMethod(reinterpret_cast<jlong>(this),
                                                      midDeqOutput,
                                                      &mid)) {
        jobject obj = env->CallObjectMethod(jHandler, mid, timeoutUs);
        jlongArray jArray = static_cast<jlongArray>(obj);
        jlong *array = env->GetLongArrayElements(jArray, 0);
        int index = static_cast<int>(array[0]);
        info.offset = static_cast<int>(array[1]);
        info.size = static_cast<int>(array[2]);
        info.presentationTimeUs = array[3];
        info.flags = static_cast<int>(array[4]);
        env->ReleaseLongArrayElements(jArray, array, 0);
        env->ExceptionCheck();
        env->ExceptionClear();
        return index;
    }
    AlLogE(TAG, "failed");
    return -1;
}

AlBuffer *AlMediaCodecBridge::getOutputBuffer(int index) {
    JNIEnv *env = nullptr;
    jmethodID mid = nullptr;
    if (AlJavaNativeHelper::getInstance()->findEnv(&env) &&
        AlJavaNativeHelper::getInstance()->findMethod(reinterpret_cast<jlong>(this),
                                                      midGetOutput,
                                                      &mid)) {
        jobject obj = env->CallObjectMethod(jHandler, mid, index);
        if (nullptr == obj) {
            AlLogE(TAG, "failed");
            return nullptr;
        }
        jlong capacity = env->GetDirectBufferCapacity(obj);
        void *buf = env->GetDirectBufferAddress(obj);
        env->ExceptionCheck();
        env->ExceptionClear();
        return AlBuffer::wrap(static_cast<uint8_t *>(buf), static_cast<size_t>(capacity));
    }
    AlLogE(TAG, "failed");
    return nullptr;
}

HwResult AlMediaCodecBridge::releaseOutputBuffer(int index, bool render) {
    JNIEnv *env = nullptr;
    jmethodID mid = nullptr;
    if (AlJavaNativeHelper::getInstance()->findEnv(&env) &&
        AlJavaNativeHelper::getInstance()->findMethod(reinterpret_cast<jlong>(this),
                                                      midReleaseOutput,
                                                      &mid)) {
        int ret = env->CallIntMethod(jHandler, mid, index, render);
        env->ExceptionCheck();
        env->ExceptionClear();
        return HwResult(ret);
    }
    AlLogE(TAG, "failed");
    return Hw::FAILED;
}

AlBuffer *AlMediaCodecBridge::getOutputFormatBuffer(std::string name) {
    JNIEnv *env = nullptr;
    jmethodID mid = nullptr;
    if (AlJavaNativeHelper::getInstance()->findEnv(&env) &&
        AlJavaNativeHelper::getInstance()->findMethod(reinterpret_cast<jlong>(this),
                                                      midGetOutFmtBuf,
                                                      &mid)) {
        const char *str = name.c_str();
        jstring jstr = env->NewStringUTF(str);
        jobject obj = env->CallObjectMethod(jHandler, mid, jstr);
        env->ReleaseStringUTFChars(jstr, str);
        if (nullptr == obj) {
            AlLogE(TAG, "failed");
            return nullptr;
        }
        jlong capacity = env->GetDirectBufferCapacity(obj);
        void *buf = env->GetDirectBufferAddress(obj);
        env->ExceptionCheck();
        env->ExceptionClear();
        return AlBuffer::wrap(static_cast<uint8_t *>(buf), static_cast<size_t>(capacity));
    }
    AlLogE(TAG, "failed");
    return nullptr;
}

int AlMediaCodecBridge::getOutputFormatInteger(std::string name) {
    JNIEnv *env = nullptr;
    jmethodID mid = nullptr;
    if (AlJavaNativeHelper::getInstance()->findEnv(&env) &&
        AlJavaNativeHelper::getInstance()->findMethod(reinterpret_cast<jlong>(this),
                                                      midGetOutFmtInt,
                                                      &mid)) {
        const char *str = name.c_str();
        jstring jstr = env->NewStringUTF(str);
        jint value = env->CallIntMethod(jHandler, mid, jstr);
        env->ReleaseStringUTFChars(jstr, str);
        return value;
    }
    AlLogE(TAG, "failed");
    return 0;
}
