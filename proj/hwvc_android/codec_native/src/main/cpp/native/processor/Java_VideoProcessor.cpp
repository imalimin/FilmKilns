/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/
#include "../include/HwJavaNativeHelper.h"
#include "HwVideoProcessor.h"
#include "../include/HwAndroidWindow.h"

#ifdef __cplusplus
extern "C" {
#endif

static JMethodDescription vPlayProgressDesc = {"Java_com_lmy_hwvcnative_processor_VideoProcessor",
                                               "onPlayProgress", "(JJ)V"};

static HwVideoProcessor *getHandler(jlong handler) {
    return reinterpret_cast<HwVideoProcessor *>(handler);
}

JNIEXPORT jlong JNICALL Java_com_lmy_hwvcnative_processor_VideoProcessor_create
        (JNIEnv *env, jobject thiz) {
    HwVideoProcessor *p = new HwVideoProcessor();
    p->post([] {
        HwJavaNativeHelper::getInstance()->attachThread();
    });
    jlong handler = reinterpret_cast<jlong>(p);
    HwJavaNativeHelper::getInstance()->registerAnObject(env, handler, thiz);
    return handler;
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_VideoProcessor_setSource
        (JNIEnv *env, jobject thiz, jlong handler, jstring path) {
    if (handler) {
        const char *pPath = env->GetStringUTFChars(path, JNI_FALSE);
        std::string pathStr(pPath);
        env->ReleaseStringUTFChars(path, pPath);
        getHandler(handler)->setSource(pathStr);
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_VideoProcessor_prepare
        (JNIEnv *env, jobject thiz, jlong handler, jobject surface) {
    if (handler) {
        getHandler(handler)->prepare(new HwAndroidWindow(env, surface));
        getHandler(handler)->setPlayProgressListener([handler](int64_t us, int64_t duration) {
            jobject jObject = nullptr;
            JNIEnv *pEnv = nullptr;
            jmethodID methodID = nullptr;
            if (HwJavaNativeHelper::getInstance()->findEnv(&pEnv) &&
                HwJavaNativeHelper::getInstance()->findJObject(handler, &jObject) &&
                HwJavaNativeHelper::getInstance()->findMethod(handler,
                                                              vPlayProgressDesc,
                                                              &methodID)) {
                pEnv->CallVoidMethod(jObject, methodID, static_cast<jlong>(us),
                                     static_cast<jlong>(duration));
            }
        });
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_VideoProcessor_updateWindow
        (JNIEnv *env, jobject thiz, jlong handler, jobject surface) {
    if (handler) {
        getHandler(handler)->updateWindow(new HwAndroidWindow(env, surface));
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_VideoProcessor_start
        (JNIEnv *env, jobject thiz, jlong handler) {
    if (handler) {
        getHandler(handler)->start();
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_VideoProcessor_pause
        (JNIEnv *env, jobject thiz, jlong handler) {
    if (handler) {
        getHandler(handler)->pause();
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_VideoProcessor_seek
        (JNIEnv *env, jobject thiz, jlong handler, jlong us) {
    if (handler) {
        getHandler(handler)->seek(us);
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_VideoProcessor_setFilter
        (JNIEnv *env, jobject thiz, jlong handler, jlong filter) {
    if (handler && filter) {
        getHandler(handler)->setFilter(reinterpret_cast<HwAbsFilter *>(filter));
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_VideoProcessor_release
        (JNIEnv *env, jobject thiz, jlong handler) {
    if (handler) {
        HwVideoProcessor *p = getHandler(handler);
        p->post([] {
            HwJavaNativeHelper::getInstance()->detachThread();
        });
        p->stop();
        delete p;
    }
    HwJavaNativeHelper::getInstance()->unregisterAnObject(env, handler);
}

#ifdef __cplusplus
}
#endif