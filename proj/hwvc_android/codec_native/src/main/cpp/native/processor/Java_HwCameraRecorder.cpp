/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/
#include "../include/HwJavaNativeHelper.h"
#include "HwCameraRecorder.h"

#ifdef __cplusplus
extern "C" {
#endif

static HwCameraRecorder *getHandler(jlong handler) {
    return reinterpret_cast<HwCameraRecorder *>(handler);
}

JNIEXPORT jlong JNICALL Java_com_lmy_hwvcnative_processor_HwCameraRecorder_create
        (JNIEnv *env, jobject thiz) {
    HwCameraRecorder *p = new HwCameraRecorder();
    p->post([] {
        HwJavaNativeHelper::getInstance()->attachThread();
    });
    jlong handler = reinterpret_cast<jlong>(p);
    HwJavaNativeHelper::getInstance()->registerAnObject(env, handler, thiz);
    return handler;
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_HwCameraRecorder_prepare
        (JNIEnv *env, jobject thiz, jlong handler) {
    if (handler) {
        getHandler(handler)->prepare();
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_HwCameraRecorder_release
        (JNIEnv *env, jobject thiz, jlong handler) {
    if (handler) {
        HwCameraRecorder *p = getHandler(handler);
        p->post([] {
            HwJavaNativeHelper::getInstance()->detachThread();
        });
        delete p;
    }
    HwJavaNativeHelper::getInstance()->unregisterAnObject(env, handler);
}

#ifdef __cplusplus
}
#endif