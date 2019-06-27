/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/
#include <jni.h>
#include <log.h>
#include <map>
#include "HwVideoProcessor.h"
#include "../include/HwAndroidWindow.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "ff/libavcodec/jni.h"

static JavaVM *mJavaVM;
static jmethodID method = nullptr;
static map<jlong, jobject> objMap;

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    mJavaVM = vm;
    av_jni_set_java_vm(vm, NULL);
    return JNI_VERSION_1_6;
}

static HwVideoProcessor *getHandler(jlong handler) {
    return reinterpret_cast<HwVideoProcessor *>(handler);
}

JNIEXPORT jlong JNICALL Java_com_lmy_hwvcnative_processor_VideoProcessor_create
        (JNIEnv *env, jobject thiz) {
    jclass clazz = env->GetObjectClass(thiz);
    if (!method) {
        method = env->GetMethodID(clazz, "onPlayProgress", "(J)V");
    }
    jlong handler = reinterpret_cast<jlong>(new HwVideoProcessor());
    if (objMap.end() == objMap.find(handler)) {
        objMap.insert(pair<jlong, jobject>(handler, env->NewGlobalRef(thiz)));
    }
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
            if (objMap.end() == objMap.find(handler)) {
                Logcat::i("HWVC", "HwVideoProcessor::play thiz is null");
                return;
            }
            if (!mJavaVM) {
                Logcat::i("HWVC", "HwVideoProcessor::play attach current thread failed a.");
                return;
            }
            JNIEnv *pEnv = nullptr;
            int status = mJavaVM->AttachCurrentThread(&pEnv, NULL);
            if (status < 0) {
                Logcat::i("HWVC", "HwVideoProcessor::play attach current thread failed b.");
                return;
            }
            pEnv->CallVoidMethod(objMap[handler], method, static_cast<jlong>(us));
            mJavaVM->DetachCurrentThread();
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
        p->stop();
        delete p;
    }
    auto itr = objMap.find(handler);
    if (objMap.end() != itr) {
        env->DeleteGlobalRef(itr->second);
        objMap.erase(itr);
    }
}

#ifdef __cplusplus
}
#endif