/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "jni.h"
#include "FkLayerEngine.h"

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jlong JNICALL Java_com_alimin_fk_engine_FkImage_nativeCreateInstance
        (JNIEnv *env, jobject that) {
    auto *p = new FkLayerEngine("FkImage");
    return reinterpret_cast<jlong>(p);
}

JNIEXPORT void JNICALL Java_com_alimin_fk_engine_FkImage_nativeCreate
        (JNIEnv *env, jobject that, jlong handle) {
    auto *engine = reinterpret_cast<FkLayerEngine *>(handle);
    engine->create();
}

JNIEXPORT void JNICALL Java_com_alimin_fk_engine_FkImage_nativeDestroy
        (JNIEnv *env, jobject that, jlong handle) {
    auto *engine = reinterpret_cast<FkLayerEngine *>(handle);
    engine->destroy();
}

JNIEXPORT void JNICALL Java_com_alimin_fk_engine_FkImage_nativeStart
        (JNIEnv *env, jobject that, jlong handle) {
    auto *engine = reinterpret_cast<FkLayerEngine *>(handle);
    engine->start();
}

JNIEXPORT void JNICALL Java_com_alimin_fk_engine_FkImage_nativeStop
        (JNIEnv *env, jobject that, jlong handle) {
    auto *engine = reinterpret_cast<FkLayerEngine *>(handle);
    engine->stop();
}

JNIEXPORT jint JNICALL Java_com_alimin_fk_engine_FkImage_nativeNewLayer
        (JNIEnv *env, jobject that, jlong handle, jstring path) {
    auto *engine = reinterpret_cast<FkLayerEngine *>(handle);
    auto *p = env->GetStringUTFChars(path, nullptr);
    auto layer = engine->newLayer(std::string(p));
    env->ReleaseStringUTFChars(path, p);
    return layer;
}

#ifdef __cplusplus
}
#endif