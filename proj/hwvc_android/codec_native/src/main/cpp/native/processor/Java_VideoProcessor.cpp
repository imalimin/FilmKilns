/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/
#include "../include/HwJavaNativeHelper.h"
#include "HwVideoProcessor.h"
#include "../include/HwAndroidWindow.h"

#ifdef __cplusplus
extern "C" {
#endif

static JMethodDescription playProgressDesc = {"onPlayProgress", "(J)V"};

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
        int len = env->GetStringUTFLength(path) + 1;
        const char *pPath = env->GetStringUTFChars(path, JNI_FALSE);
        char *str = new char[len];
        memcpy(str, pPath, len);
        getHandler(handler)->setSource(str);
        env->ReleaseStringUTFChars(path, pPath);
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_VideoProcessor_prepare
        (JNIEnv *env, jobject thiz, jlong handler, jobject surface) {
    if (handler) {
        getHandler(handler)->prepare(new HwAndroidWindow(env, surface));
        getHandler(handler)->setPlayProgressListener([handler](int64_t us) {
            jobject jObject;
            JNIEnv *pEnv = nullptr;
            jmethodID methodID = nullptr;
            if (HwJavaNativeHelper::getInstance()->findEnv(&pEnv) &&
                HwJavaNativeHelper::getInstance()->findJObject(handler, &jObject) &&
                HwJavaNativeHelper::getInstance()->findMethod(handler,
                                                              playProgressDesc,
                                                              &methodID)) {
                pEnv->CallVoidMethod(jObject, methodID, static_cast<jlong>(us));
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
        getHandler(handler)->setFilter(reinterpret_cast<Filter *>(filter));
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