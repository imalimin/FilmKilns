/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "jni.h"
#include "FkImageEngine.h"
#include "FkImageFileEngine.h"
#include "FkInstanceHolder.h"

#define FILE_ENGINE_ALIAS "FileEngine"

#ifdef __cplusplus
extern "C" {
#endif

static std::shared_ptr<FkImageFileEngine> castHandle(jlong handle) {
    return FkInstanceHolder::getInstance().find<std::shared_ptr<FkImageFileEngine>>(handle);
}

JNIEXPORT jlong JNICALL Java_com_alimin_fk_engine_FkImageFile_nativeCreateInstance
        (JNIEnv *env, jobject that, jlong imageEngineHandle) {
    auto imageEngine = FkInstanceHolder::getInstance().find<std::shared_ptr<FkImageEngine>>(imageEngineHandle);
    auto engine = std::dynamic_pointer_cast<FkEngine>(imageEngine);
    auto fileEngine = std::make_shared<FkImageFileEngine>(engine, FILE_ENGINE_ALIAS);
    return FkInstanceHolder::getInstance().put(fileEngine);
}

JNIEXPORT void JNICALL Java_com_alimin_fk_engine_FkImageFile_nativeCreate
        (JNIEnv *env, jobject that, jlong handle) {
    auto engine = castHandle(handle);
    engine->create();
}

JNIEXPORT void JNICALL Java_com_alimin_fk_engine_FkImageFile_nativeDestroy
        (JNIEnv *env, jobject that, jlong handle) {
    auto engine = castHandle(handle);
    engine->destroy();
    FkInstanceHolder::getInstance().release(handle);
}

JNIEXPORT void JNICALL Java_com_alimin_fk_engine_FkImageFile_nativeStart
        (JNIEnv *env, jobject that, jlong handle) {
    auto engine = castHandle(handle);
    engine->start();
}

JNIEXPORT void JNICALL Java_com_alimin_fk_engine_FkImageFile_nativeStop
        (JNIEnv *env, jobject that, jlong handle) {
    auto engine = castHandle(handle);
    engine->stop();
}

JNIEXPORT jint JNICALL Java_com_alimin_fk_engine_FkImageFile_nativeSave
        (JNIEnv *env, jobject that, jlong handle, jstring file) {
    auto pFile = env->GetStringUTFChars(file, nullptr);
    std::string fileStr(pFile);
    env->ReleaseStringUTFChars(file, pFile);

    auto engine = castHandle(handle);
    return engine->save(fileStr);
}

JNIEXPORT jint JNICALL Java_com_alimin_fk_engine_FkImageFile_nativeLoad
        (JNIEnv *env, jobject that, jlong handle, jstring file) {
    auto pFile = env->GetStringUTFChars(file, nullptr);
    std::string fileStr(pFile);
    env->ReleaseStringUTFChars(file, pFile);

    auto engine = castHandle(handle);
    return engine->load(fileStr);
}

#ifdef __cplusplus
}
#endif
