/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include <jni.h>
#include <log.h>
#include "PictureProcessor.h"
#include <android/native_window_jni.h>
#include "../include/HwAndroidWindow.h"

#ifdef __cplusplus
extern "C" {
#endif

static PictureProcessor *getHandler(jlong handler) {
    return reinterpret_cast<PictureProcessor *>(handler);
}

JNIEXPORT jlong JNICALL Java_com_lmy_hwvcnative_processor_PictureProcessor_create
        (JNIEnv *env, jobject thiz) {
    return reinterpret_cast<jlong>(new PictureProcessor());
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_PictureProcessor_prepare
        (JNIEnv *env, jobject thiz, jlong handler, jobject surface) {
    if (handler) {
        getHandler(handler)->prepare(new HwAndroidWindow(env, surface));
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_PictureProcessor_updateWindow
        (JNIEnv *env, jobject thiz, jlong handler, jobject surface) {
    if (handler) {
        getHandler(handler)->updateWindow(new HwAndroidWindow(env, surface));
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_PictureProcessor_show
        (JNIEnv *env, jobject thiz, jlong handler, jstring file) {
    if (handler) {
        const char *pPath = env->GetStringUTFChars(file, JNI_FALSE);
        std::string pathStr(pPath);
        env->ReleaseStringUTFChars(file, pPath);
        getHandler(handler)->show(pathStr);
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_PictureProcessor_release
        (JNIEnv *env, jobject thiz, jlong handler) {
    if (handler) {
        PictureProcessor *p = getHandler(handler);
        delete p;
        LOGI("%s", __FUNCTION__);
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_PictureProcessor_setFilter
        (JNIEnv *env, jobject thiz, jlong handler, jlong filter) {
    if (handler && filter) {
        getHandler(handler)->setFilter(reinterpret_cast<HwAbsFilter *>(filter));
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_PictureProcessor_invalidate
        (JNIEnv *env, jobject thiz, jlong handler) {
    if (handler) {
        getHandler(handler)->invalidate();
    }
}

#ifdef __cplusplus
}
#endif