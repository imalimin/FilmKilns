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

#include "ff/libavcodec/jni.h"

static HwJavaNativeHelper helper;
static JMethodDescription playProgressDesc = {"onPlayProgress", "(J)V"};

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    helper.attach(vm);
    av_jni_set_java_vm(vm, NULL);
    return JNI_VERSION_1_6;
}

JNIEXPORT void JNI_OnUnload(JavaVM *vm, void *reserved) {
    helper.detach();
}

static HwVideoProcessor *getHandler(jlong handler) {
    return reinterpret_cast<HwVideoProcessor *>(handler);
}

JNIEXPORT jlong JNICALL Java_com_lmy_hwvcnative_processor_VideoProcessor_create
        (JNIEnv *env, jobject thiz) {
    HwVideoProcessor *p = new HwVideoProcessor();
    p->post([] {
        helper.attachThread();
    });
    jlong handler = reinterpret_cast<jlong>(p);
    helper.registerAnObject(env, handler, thiz);
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
            if (helper.findEnv(&pEnv) &&
                helper.findJObject(handler, &jObject) &&
                helper.findMethod(handler, playProgressDesc, &methodID)) {
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
            helper.detachThread();
        });
        p->stop();
        delete p;
    }
    helper.unregisterAnObject(env, handler);
}

#ifdef __cplusplus
}
#endif