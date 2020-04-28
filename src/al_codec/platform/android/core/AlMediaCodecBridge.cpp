/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlMediaCodecBridge.h"

#define TAG "AlMediaCodecBridge"

const JMethodDescription AlMediaCodecBridge::midInit = {
        "com/lmy/hwvcnative/core/AlMediaCodec",
        "<init>", "(IZZ)V"};
const JMethodDescription AlMediaCodecBridge::midConfigure = {
        "com/lmy/hwvcnative/core/AlMediaCodec",
        "configure", "(IIIIII)Z"};
const JMethodDescription AlMediaCodecBridge::midGetExtraBuffer = {
        "com/lmy/hwvcnative/core/AlMediaCodec",
        "getExtraBuffer", "(Ljava/lang/String;)Ljava/nio/ByteBuffer;"};
const JMethodDescription AlMediaCodecBridge::midFlush = {
        "com/lmy/hwvcnative/core/AlMediaCodec",
        "flush", "()V"};
const JMethodDescription AlMediaCodecBridge::midRelease = {
        "com/lmy/hwvcnative/core/AlMediaCodec",
        "release", "()V"};
const JMethodDescription AlMediaCodecBridge::midStart = {
        "com/lmy/hwvcnative/core/AlMediaCodec",
        "start", "()Z"};
const JMethodDescription AlMediaCodecBridge::midProcess = {
        "com/lmy/hwvcnative/core/AlMediaCodec",
        "process", "(Ljava/nio/ByteBuffer;J)I"};
const JMethodDescription AlMediaCodecBridge::midType = {
        "com/lmy/hwvcnative/core/AlMediaCodec",
        "type", "()I"};
const JMethodDescription AlMediaCodecBridge::midGetBuffer = {
        "com/lmy/hwvcnative/core/AlMediaCodec",
        "getBuffer", "()Ljava/nio/ByteBuffer;"};
const JMethodDescription AlMediaCodecBridge::midGetBufferInfo = {
        "com/lmy/hwvcnative/core/AlMediaCodec",
        "getBufferInfo", "()[J"};

AlMediaCodecBridge::AlMediaCodecBridge(int32_t codecId, bool makeNalSelf)
        : Object(), codecId(codecId), makeNalSelf(makeNalSelf) {
    JNIEnv *env = nullptr;
    if (!AlJavaNativeHelper::getInstance()->findEnv(&env)) {
        AlLogI(TAG, "failed");
        return;
    }
    jclass cls = AlJavaNativeHelper::getInstance()->registerAnClass(midInit.cls.c_str());
    if (nullptr == cls) {
        AlLogI(TAG, "failed");
        return;
    }
    auto mid = env->GetMethodID(cls, midInit.name.c_str(), midInit.sign.c_str());
    if (nullptr == mid) {
        AlLogI(TAG, "failed");
        return;
    }
    jHandler = env->NewObject(cls, mid, codecId, true, makeNalSelf);
    env->ExceptionCheck();
    env->ExceptionClear();
    if (nullptr == jHandler) {
        AlLogI(TAG, "failed");
        return;
    }
    AlJavaNativeHelper::getInstance()->registerAnObject(env,
                                                        reinterpret_cast<jlong>(this),
                                                        jHandler);
    AlLogI(TAG, "success");
}

AlMediaCodecBridge::~AlMediaCodecBridge() {
    for (auto &buffer : buffers) {
        delete buffer;
        buffer = nullptr;
    }
    JNIEnv *env = nullptr;
    if (AlJavaNativeHelper::getInstance()->findEnv(&env)) {
        env->DeleteLocalRef(jHandler);
        AlJavaNativeHelper::getInstance()->unregisterAnObject(env, reinterpret_cast<jlong>(this));
    }
}

HwResult AlMediaCodecBridge::configure(int w, int h, int bitrate,
                                       int format, int iFrameInterval, int fps) {
    JNIEnv *env = nullptr;
    jmethodID mid = nullptr;
    bool ret = false;
    if (AlJavaNativeHelper::getInstance()->findEnv(&env) &&
        AlJavaNativeHelper::getInstance()->findMethod(reinterpret_cast<jlong>(this),
                                                      midConfigure,
                                                      &mid)) {
        ret = env->CallBooleanMethod(jHandler, mid, w, h, bitrate, format, iFrameInterval, fps);
    }
    return ret ? Hw::SUCCESS : Hw::FAILED;
}

HwBuffer *AlMediaCodecBridge::getExtraBuffer(std::string key) {
    int index = 0;
    if (HwAbsCodec::KEY_CSD_0 == key) {
        index = 0;
    } else if (HwAbsCodec::KEY_CSD_1 == key) {
        index = 1;
    } else if (HwAbsCodec::KEY_CSD_2 == key) {
        index = 2;
    } else if (HwAbsCodec::KEY_CSD_3 == key) {
        index = 3;
    }
    if (buffers[index]) {
        return buffers[index];
    }

    JNIEnv *env = nullptr;
    jmethodID mid = nullptr;
    if (AlJavaNativeHelper::getInstance()->findEnv(&env) &&
        AlJavaNativeHelper::getInstance()->findMethod(reinterpret_cast<jlong>(this),
                                                      midGetExtraBuffer,
                                                      &mid)) {
        const char *str = key.c_str();
        jstring jstr = env->NewStringUTF(str);
        jobject dbuf = env->CallObjectMethod(jHandler, mid, jstr);
        env->ReleaseStringUTFChars(jstr, str);

        jlong capacity = env->GetDirectBufferCapacity(dbuf);
        void *buf = env->GetDirectBufferAddress(dbuf);
        if (buf) {
            buffers[index] = HwBuffer::alloc(capacity);
            buffers[index]->put(static_cast<uint8_t *>(buf), capacity);
            return buffers[index];
        }
    }
    return nullptr;
}

void AlMediaCodecBridge::flush() {
    JNIEnv *env = nullptr;
    jmethodID mid = nullptr;
    if (AlJavaNativeHelper::getInstance()->findEnv(&env) &&
        AlJavaNativeHelper::getInstance()->findMethod(reinterpret_cast<jlong>(this),
                                                      midFlush,
                                                      &mid)) {
        env->CallVoidMethod(jHandler, mid);
    }
}

void AlMediaCodecBridge::release() {
    JNIEnv *env = nullptr;
    jmethodID mid = nullptr;
    if (AlJavaNativeHelper::getInstance()->findEnv(&env) &&
        AlJavaNativeHelper::getInstance()->findMethod(reinterpret_cast<jlong>(this),
                                                      midRelease,
                                                      &mid)) {
        env->CallVoidMethod(jHandler, mid);
    }
}

bool AlMediaCodecBridge::start() {
    JNIEnv *env = nullptr;
    jmethodID mid = nullptr;
    if (AlJavaNativeHelper::getInstance()->findEnv(&env) &&
        AlJavaNativeHelper::getInstance()->findMethod(reinterpret_cast<jlong>(this),
                                                      midStart,
                                                      &mid)) {
        return JNI_TRUE == env->CallBooleanMethod(jHandler, mid);
    }
    return false;
}

HwResult AlMediaCodecBridge::process(AlBuffer *buf, int64_t pts) {
    JNIEnv *env = nullptr;
    jmethodID mid = nullptr;
    if (AlJavaNativeHelper::getInstance()->findEnv(&env) &&
        AlJavaNativeHelper::getInstance()->findMethod(reinterpret_cast<jlong>(this),
                                                      midProcess,
                                                      &mid)) {
        jobject jbuf = env->NewDirectByteBuffer(buf->data(), buf->size());
        int ret = env->CallIntMethod(jHandler, mid, jbuf, pts);
        return HwResult(ret);
    }
    return Hw::FAILED;
}

int32_t AlMediaCodecBridge::type() {
    JNIEnv *env = nullptr;
    jmethodID mid = nullptr;
    if (AlJavaNativeHelper::getInstance()->findEnv(&env) &&
        AlJavaNativeHelper::getInstance()->findMethod(reinterpret_cast<jlong>(this),
                                                      midType,
                                                      &mid)) {
        return env->CallIntMethod(jHandler, mid);
    }
    return Hw::FAILED.code;
}

HwBuffer *AlMediaCodecBridge::getBuffer() {
    JNIEnv *env = nullptr;
    jmethodID mid = nullptr;
    if (AlJavaNativeHelper::getInstance()->findEnv(&env) &&
        AlJavaNativeHelper::getInstance()->findMethod(reinterpret_cast<jlong>(this),
                                                      midGetBuffer,
                                                      &mid)) {
        jobject obj = env->CallObjectMethod(jHandler, mid);
        if (obj) {
            uint8_t *buf = static_cast<uint8_t *>(env->GetDirectBufferAddress(obj));
            return HwBuffer::wrap(buf, env->GetDirectBufferCapacity(obj));
        }
    }
    return nullptr;
}

void AlMediaCodecBridge::getBufferInfo(size_t &size, int64_t &presentationTimeUs, bool &keyFrame) {
    size = 0;
    presentationTimeUs = 0;
    keyFrame = -1;
    JNIEnv *env = nullptr;
    jmethodID mid = nullptr;
    if (AlJavaNativeHelper::getInstance()->findEnv(&env) &&
        AlJavaNativeHelper::getInstance()->findMethod(reinterpret_cast<jlong>(this),
                                                      midGetBufferInfo,
                                                      &mid)) {
        jobject obj = env->CallObjectMethod(jHandler, mid);
        if (obj) {
            jlongArray jArray = static_cast<jlongArray>(obj);
            jlong *array = env->GetLongArrayElements(jArray, 0);
            size = static_cast<size_t>(array[0]);
            presentationTimeUs = array[1];
            keyFrame = 0 == array[2];
            env->ReleaseLongArrayElements(jArray, array, 0);
        }
    }
}
