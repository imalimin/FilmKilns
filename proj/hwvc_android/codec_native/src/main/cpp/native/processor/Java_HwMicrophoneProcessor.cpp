/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "jni.h"
#include "HwMicrophoneProcessor.h"
#include "HwSampleFormat.h"

#ifdef __cplusplus
extern "C" {
#endif

static HwMicrophoneProcessor *getHandler(jlong handler) {
    return reinterpret_cast<HwMicrophoneProcessor *>(handler);
}

JNIEXPORT jlong JNICALL Java_com_lmy_hwvcnative_processor_HwMicrophoneProcessor_create
        (JNIEnv *env, jobject thiz) {
    HwMicrophoneProcessor *p = new HwMicrophoneProcessor();
    jlong handler = reinterpret_cast<jlong>(p);
    return handler;
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_HwMicrophoneProcessor_prepare
        (JNIEnv *env, jobject thiz, jlong handler, jstring filePath) {
    const char *pPilePath = env->GetStringUTFChars(filePath, JNI_FALSE);
    std::string pathStr(pPilePath);
    env->ReleaseStringUTFChars(filePath, pPilePath);
    if (handler) {
        getHandler(handler)->prepare(pathStr,
                                     HwSampleFormat(HwFrameFormat::HW_SAMPLE_S32, 2, 44100));
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_HwMicrophoneProcessor_start
        (JNIEnv *env, jobject thiz, jlong handler) {
    if (handler) {
        getHandler(handler)->start();
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_HwMicrophoneProcessor_pause
        (JNIEnv *env, jobject thiz, jlong handler) {
    if (handler) {
        getHandler(handler)->pause();
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_HwMicrophoneProcessor_release
        (JNIEnv *env, jobject thiz, jlong handler) {
    if (handler) {
        HwMicrophoneProcessor *p = getHandler(handler);
        delete p;
    }
}

#ifdef __cplusplus
}
#endif
