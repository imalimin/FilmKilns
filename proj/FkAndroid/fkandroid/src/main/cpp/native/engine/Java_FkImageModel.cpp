/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "jni.h"
#include "FkImageEngine.h"
#include "FkImageModelEngine.h"
#include "FkInstanceHolder.h"
#include "FkJniDefinition.h"
#include "FkJavaFunc.h"

#define FILE_ENGINE_ALIAS "FileEngine"

#define FK_CLASS Java_com_alimin_fk_engine_FkImageModel

#ifdef __cplusplus
extern "C" {
#endif

static std::shared_ptr<FkImageModelEngine> castHandle(jlong handle) {
    return FkInstanceHolder::getInstance().find<std::shared_ptr<FkImageModelEngine>>(handle);
}

JNIEXPORT jlong JNICALL FK_JNI_DEFINE(nativeCreateInstance)
        (JNIEnv *env, jobject that, jlong imageEngineHandle) {
    auto imageEngine = FkInstanceHolder::getInstance().find<std::shared_ptr<FkImageEngine>>(
            imageEngineHandle);
    auto engine = std::dynamic_pointer_cast<FkEngine>(imageEngine);
    auto fileEngine = std::make_shared<FkImageModelEngine>(engine, FILE_ENGINE_ALIAS);
    return FkInstanceHolder::getInstance().put(fileEngine);
}

JNIEXPORT void JNICALL FK_JNI_DEFINE(nativeCreate)
        (JNIEnv *env, jobject that, jlong handle) {
    auto engine = castHandle(handle);
    FkJavaFunc func(env, that, "onNativeMsgReceived", "(ILjava/nio/ByteBuffer;)B");
    engine->create();
}

JNIEXPORT void JNICALL FK_JNI_DEFINE(nativeDestroy)
        (JNIEnv *env, jobject that, jlong handle) {
    auto engine = castHandle(handle);
    engine->destroy();
    FkInstanceHolder::getInstance().release(handle);
}

JNIEXPORT void JNICALL FK_JNI_DEFINE(nativeStart)
        (JNIEnv *env, jobject that, jlong handle) {
    auto engine = castHandle(handle);
    engine->start();
}

JNIEXPORT void JNICALL FK_JNI_DEFINE(nativeStop)
        (JNIEnv *env, jobject that, jlong handle) {
    auto engine = castHandle(handle);
    engine->stop();
}

JNIEXPORT jint JNICALL FK_JNI_DEFINE(nativeSave)
        (JNIEnv *env, jobject that, jlong handle, jstring file) {
    auto pFile = env->GetStringUTFChars(file, nullptr);
    std::string fileStr(pFile);
    env->ReleaseStringUTFChars(file, pFile);

    auto engine = castHandle(handle);
    return engine->save(fileStr);
}

JNIEXPORT jint JNICALL FK_JNI_DEFINE(nativeLoad)
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
