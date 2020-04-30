/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlMediaCodecBridge.h"

#define TAG "AlMediaCodecBridge"

const AlJNIObject::Method AlMediaCodecBridge::midInit = {
        "com/lmy/hwvcnative/core/AlMediaCodecKt",
        "<init>", "(Ljava/lang/String;)V"};
const AlJNIObject::Method AlMediaCodecBridge::midConfigure = {
        "com/lmy/hwvcnative/core/AlMediaCodecKt",
        "configure", "(IIIIII)I"};
const AlJNIObject::Method AlMediaCodecBridge::midStart = {
        "com/lmy/hwvcnative/core/AlMediaCodecKt",
        "start", "()I"};
const AlJNIObject::Method AlMediaCodecBridge::midStop = {
        "com/lmy/hwvcnative/core/AlMediaCodecKt",
        "stop", "()I"};
const AlJNIObject::Method AlMediaCodecBridge::midFlush = {
        "com/lmy/hwvcnative/core/AlMediaCodecKt",
        "flush", "()I"};
const AlJNIObject::Method AlMediaCodecBridge::midRelease = {
        "com/lmy/hwvcnative/core/AlMediaCodecKt",
        "release", "()V"};
const AlJNIObject::Method AlMediaCodecBridge::midDeqInput = {
        "com/lmy/hwvcnative/core/AlMediaCodecKt",
        "dequeueInputBuffer", "(J)I"};
const AlJNIObject::Method AlMediaCodecBridge::midQueInput = {
        "com/lmy/hwvcnative/core/AlMediaCodecKt",
        "queueInputBuffer", "(IIIJI)I"};
const AlJNIObject::Method AlMediaCodecBridge::midGetInput = {
        "com/lmy/hwvcnative/core/AlMediaCodecKt",
        "getInputBuffer", "(I)Ljava/nio/ByteBuffer;"};
const AlJNIObject::Method AlMediaCodecBridge::midDeqOutput = {
        "com/lmy/hwvcnative/core/AlMediaCodecKt",
        "dequeueOutputBuffer", "(J)[J"};
const AlJNIObject::Method AlMediaCodecBridge::midGetOutput = {
        "com/lmy/hwvcnative/core/AlMediaCodecKt",
        "getOutputBuffer", "(I)Ljava/nio/ByteBuffer;"};
const AlJNIObject::Method AlMediaCodecBridge::midReleaseOutput = {
        "com/lmy/hwvcnative/core/AlMediaCodecKt",
        "releaseOutputBuffer", "(IB)I"};
const AlJNIObject::Method AlMediaCodecBridge::midGetOutFmtBuf = {
        "com/lmy/hwvcnative/core/AlMediaCodecKt",
        "getOutputFormatBuffer", "(Ljava/lang/String;)Ljava/nio/ByteBuffer;"};
const AlJNIObject::Method AlMediaCodecBridge::midGetOutFmtInt = {
        "com/lmy/hwvcnative/core/AlMediaCodecKt",
        "getOutputFormatInteger", "(Ljava/lang/String;)I"};

AlMediaCodecBridge::Info::Info() {

}

AlMediaCodecBridge::Info::~Info() {

}

AlMediaCodecBridge::AlMediaCodecBridge(const std::string &mime) : Object() {
    JNIEnv *env = nullptr;
    if (!AlJNIEnv::getInstance().findEnv(&env)) {
        AlLogE(TAG, "failed");
        return;
    }

    auto cls = AlJNIEnv::getInstance().registerAnClass(midInit.cls.c_str());
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
    jobject handle = env->NewObject(cls, mid, jstr);
    env->ReleaseStringUTFChars(jstr, str);
    env->ExceptionCheck();
    env->ExceptionClear();
    if (nullptr == handle) {
        AlLogE(TAG, "failed");
        return;
    }
    if (AlJNIEnv::getInstance().attach(this, handle)) {
        AlLogI(TAG, "success");
    }
    env->DeleteLocalRef(handle);
}

AlMediaCodecBridge::~AlMediaCodecBridge() {
    AlJNIEnv::getInstance().detach(this);
}

HwResult AlMediaCodecBridge::configure(int w, int h,
                                       int bitrate,
                                       int format,
                                       int iFrameInterval,
                                       int fps) {
    AlLogI(TAG, "enter.");
    AlJNIObject *obj = nullptr;
    if (AlJNIEnv::getInstance().findObj(this, &obj)) {
        al_jni_call_int(obj, midConfigure, ret, w, h, bitrate, format, iFrameInterval, fps);
        return HwResult(ret);
    }
    AlLogE(TAG, "failed");
    return Hw::FAILED;
}

HwResult AlMediaCodecBridge::start() {
    AlJNIObject *obj = nullptr;
    if (AlJNIEnv::getInstance().findObj(this, &obj)) {
        al_jni_call_int(obj, midStart, ret);
        return HwResult(ret);
    }
    return Hw::FAILED;
}

HwResult AlMediaCodecBridge::stop() {
    AlJNIObject *obj = nullptr;
    if (AlJNIEnv::getInstance().findObj(this, &obj)) {
        al_jni_call_int(obj, midStop, ret);
        return HwResult(ret);
    }
    AlLogE(TAG, "failed");
    return Hw::FAILED;
}

HwResult AlMediaCodecBridge::flush() {
    AlJNIObject *obj = nullptr;
    if (AlJNIEnv::getInstance().findObj(this, &obj)) {
        al_jni_call_int(obj, midFlush, ret);
        return HwResult(ret);
    }
    AlLogE(TAG, "failed");
    return Hw::FAILED;
}

void AlMediaCodecBridge::release() {
    AlJNIObject *obj = nullptr;
    if (AlJNIEnv::getInstance().findObj(this, &obj)) {
        al_jni_call_void(obj, midRelease);
    }
    AlLogE(TAG, "failed");
}

int AlMediaCodecBridge::dequeueInputBuffer(long timeoutUs) {
    AlJNIObject *obj = nullptr;
    if (AlJNIEnv::getInstance().findObj(this, &obj)) {
        al_jni_call_int(obj, midDeqInput, ret, timeoutUs);
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
    AlJNIObject *obj = nullptr;
    if (AlJNIEnv::getInstance().findObj(this, &obj)) {
        al_jni_call_int(obj, midQueInput, ret, index, offset, size, presentationTimeUs, flags);
        return HwResult(ret);
    }
    AlLogE(TAG, "failed");
    return Hw::SUCCESS;
}

AlBuffer *AlMediaCodecBridge::getInputBuffer(int index) {
    AlJNIObject *obj = nullptr;
    if (AlJNIEnv::getInstance().findObj(this, &obj)) {
        al_jni_call_buffer(obj, midGetInput, buf, index);
        return buf;
    }
    AlLogE(TAG, "failed");
    return nullptr;
}

int AlMediaCodecBridge::dequeueOutputBuffer(AlMediaCodecBridge::Info &info, long timeoutUs) {
    AlJNIObject *obj = nullptr;
    if (AlJNIEnv::getInstance().findObj(this, &obj)) {
        al_jni_call_long_array(obj, midDeqOutput, v, timeoutUs);
        info.offset = static_cast<int>(v[1]);
        info.size = static_cast<int>(v[2]);
        info.presentationTimeUs = v[3];
        info.flags = static_cast<int>(v[4]);
        return static_cast<int>(v[0]);
    }
    AlLogE(TAG, "failed");
    return -1;
}

AlBuffer *AlMediaCodecBridge::getOutputBuffer(int index) {
    AlJNIObject *obj = nullptr;
    if (AlJNIEnv::getInstance().findObj(this, &obj)) {
        al_jni_call_buffer(obj, midGetOutput, buf, index);
        return buf;
    }
    AlLogE(TAG, "failed");
    return nullptr;
}

HwResult AlMediaCodecBridge::releaseOutputBuffer(int index, bool render) {
    AlJNIObject *obj = nullptr;
    if (AlJNIEnv::getInstance().findObj(this, &obj)) {
        al_jni_call_int(obj, midReleaseOutput, ret, index, render);
        return HwResult(ret);
    }
    AlLogE(TAG, "failed");
    return Hw::FAILED;
}

AlBuffer *AlMediaCodecBridge::getOutputFormatBuffer(std::string name) {
    JNIEnv *env = nullptr;
    if (!AlJNIEnv::getInstance().findEnv(&env)) {
        AlLogE(TAG, "failed");
        return nullptr;
    }
    AlJNIObject *obj = nullptr;
    if (AlJNIEnv::getInstance().findObj(this, &obj)) {
        const char *str = name.c_str();
        jstring jstr = env->NewStringUTF(str);
        al_jni_call_buffer(obj, midGetOutFmtBuf, buf, jstr);
        env->ReleaseStringUTFChars(jstr, str);
        return buf;
    }
    AlLogE(TAG, "failed");
    return nullptr;
}

int AlMediaCodecBridge::getOutputFormatInteger(std::string name) {
    JNIEnv *env = nullptr;
    if (!AlJNIEnv::getInstance().findEnv(&env)) {
        AlLogE(TAG, "failed");
        return 0;
    }
    AlJNIObject *obj = nullptr;
    if (AlJNIEnv::getInstance().findObj(this, &obj)) {
        const char *str = name.c_str();
        jstring jstr = env->NewStringUTF(str);
        al_jni_call_int(obj, midGetOutFmtInt, ret, jstr)
        env->ReleaseStringUTFChars(jstr, str);
        return ret;
    }
    AlLogE(TAG, "failed");
    return 0;
}
