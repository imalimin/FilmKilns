/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "jni.h"
#include "android/native_window.h"
#include "android/native_window_jni.h"
#include "FkImageEngine.h"
#include "FkGraphicWindow.h"

#ifdef __cplusplus
extern "C" {
#endif

static FkImageEngine *castHandle(jlong handle) {
    return reinterpret_cast<FkImageEngine *>(handle);
}

JNIEXPORT jlong JNICALL Java_com_alimin_fk_engine_FkImage_nativeCreateInstance
        (JNIEnv *env, jobject that) {
    auto *p = new FkImageEngine("FkImage");
    return reinterpret_cast<jlong>(p);
}

JNIEXPORT void JNICALL Java_com_alimin_fk_engine_FkImage_nativeCreate
        (JNIEnv *env, jobject that, jlong handle) {
    auto *engine = castHandle(handle);
    engine->create();
}

JNIEXPORT void JNICALL Java_com_alimin_fk_engine_FkImage_nativeDestroy
        (JNIEnv *env, jobject that, jlong handle) {
    auto *engine = castHandle(handle);
    engine->destroy();
}

JNIEXPORT void JNICALL Java_com_alimin_fk_engine_FkImage_nativeStart
        (JNIEnv *env, jobject that, jlong handle) {
    auto *engine = castHandle(handle);
    engine->start();
}

JNIEXPORT void JNICALL Java_com_alimin_fk_engine_FkImage_nativeStop
        (JNIEnv *env, jobject that, jlong handle) {
    auto *engine = castHandle(handle);
    engine->stop();
}

JNIEXPORT jint JNICALL Java_com_alimin_fk_engine_FkImage_nativeSetSurface
        (JNIEnv *env, jobject that, jlong handle, jobject surface) {
    auto *engine = castHandle(handle);
    if (surface) {
        auto nativeHandle = ANativeWindow_fromSurface(env, surface);
        auto win = std::make_shared<FkGraphicWindow>(nativeHandle,
                                                     ANativeWindow_getWidth(nativeHandle),
                                                     ANativeWindow_getHeight(nativeHandle));
        return engine->setSurface(win);
    } else {
        return engine->setSurface(nullptr);
    }
}

JNIEXPORT jint JNICALL Java_com_alimin_fk_engine_FkImage_nativeNewLayerWithFile
        (JNIEnv *env, jobject that, jlong handle, jstring path) {
    auto *engine = castHandle(handle);
    auto *p = env->GetStringUTFChars(path, nullptr);
    auto layer = engine->newLayerWithFile(std::string(p));
    env->ReleaseStringUTFChars(path, p);
    return layer;
}

JNIEXPORT jint JNICALL Java_com_alimin_fk_engine_FkImage_nativeNewLayerWithColor
        (JNIEnv *env, jobject that, jlong handle, jint widht, jint height, jint red, jint green,
         jint blue, jint alpha) {
    FkColor color;
    color.format = FkColor::kFormat::RGBA;
    color.red = red;
    color.greed = green;
    color.blue = blue;
    color.alpha = alpha;
    auto *engine = castHandle(handle);
    return engine->newLayerWithColor(FkSize(widht, height), color);
}

JNIEXPORT jint JNICALL Java_com_alimin_fk_engine_FkImage_nativeNotifyRender
        (JNIEnv *env, jobject that, jlong handle) {
    auto *engine = castHandle(handle);
    return engine->notifyRender();
}

#ifdef __cplusplus
}
#endif