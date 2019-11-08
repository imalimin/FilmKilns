/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include <jni.h>
#include <log.h>
#include "HwNormalFilter.h"

#ifdef __cplusplus
extern "C" {
#endif

static HwNormalFilter *getHandler(jlong handler) {
    return reinterpret_cast<HwNormalFilter *>(handler);
}

JNIEXPORT jlong JNICALL Java_com_lmy_hwvcnative_filter_HwNormalFilter_create
        (JNIEnv *env, jobject thiz) {
    return reinterpret_cast<jlong>(new HwNormalFilter());
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_filter_HwNormalFilter_setParams
        (JNIEnv *env, jobject thiz, jlong handler, jintArray params) {
    if (handler) {
        int *pParams = env->GetIntArrayElements(params, JNI_FALSE);
        getHandler(handler)->setParams(pParams);
        env->ReleaseIntArrayElements(params, pParams, 0);
    }
}

#ifdef __cplusplus
}
#endif