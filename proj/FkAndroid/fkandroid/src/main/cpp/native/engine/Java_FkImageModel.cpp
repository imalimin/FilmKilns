/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "jni.h"
#include "FkJniDefinition.h"
#include "FkImageEngine.h"
#include "FkImageModelEngine.h"
#include "FkInstanceHolder.h"
#include "FkJavaFunc.h"
#include "FkJavaRuntime.h"
#include "FkJniGlobalRef.h"

#define FILE_ENGINE_ALIAS "FileEngine"

using namespace com::alimin::fk;

#ifdef __cplusplus
extern "C" {
#endif

static std::shared_ptr<FkImageModelEngine> castHandle(jlong handle) {
    return FkInstanceHolder::getInstance().find<std::shared_ptr<FkImageModelEngine>>(handle);
}

JNIEXPORT jlong FK_JNI_METHOD_DEFINE(com_alimin_fk_engine, FkImageModel, nativeCreateInstance)
        (JNIEnv *env, jobject that, jlong imageEngineHandle) {
    auto imageEngine = FkInstanceHolder::getInstance().find<std::shared_ptr<FkImageEngine>>(
            imageEngineHandle);
    auto fileEngine = std::make_shared<FkImageModelEngine>(imageEngine, FILE_ENGINE_ALIAS);
    return FkInstanceHolder::getInstance().put(fileEngine);
}

JNIEXPORT void FK_JNI_METHOD_DEFINE(com_alimin_fk_engine, FkImageModel, nativeCreate)
        (JNIEnv *env, jobject that, jlong handle) {
    auto engine = castHandle(handle);
    engine->create();
}

JNIEXPORT void FK_JNI_METHOD_DEFINE(com_alimin_fk_engine, FkImageModel, nativeDestroy)
        (JNIEnv *env, jobject that, jlong handle) {
    auto engine = castHandle(handle);
    engine->destroy();
    FkInstanceHolder::getInstance().release(handle);
}

JNIEXPORT void FK_JNI_METHOD_DEFINE(com_alimin_fk_engine, FkImageModel, nativeStart)
        (JNIEnv *env, jobject that, jlong handle) {
    auto engine = castHandle(handle);
    engine->start();
}

JNIEXPORT void FK_JNI_METHOD_DEFINE(com_alimin_fk_engine, FkImageModel, nativeStop)
        (JNIEnv *env, jobject that, jlong handle) {
    auto engine = castHandle(handle);
    engine->stop();
}

JNIEXPORT jint FK_JNI_METHOD_DEFINE(com_alimin_fk_engine, FkImageModel, nativeSave)
        (JNIEnv *env, jobject that, jlong handle, jstring file) {
    auto pFile = env->GetStringUTFChars(file, nullptr);
    std::string fileStr(pFile);
    env->ReleaseStringUTFChars(file, pFile);

    auto engine = castHandle(handle);
    return engine->save(fileStr);
}

JNIEXPORT jint FK_JNI_METHOD_DEFINE(com_alimin_fk_engine, FkImageModel, nativeLoad)
        (JNIEnv *env, jobject that, jlong handle, jstring file) {
    auto pFile = env->GetStringUTFChars(file, nullptr);
    std::string fileStr(pFile);
    env->ReleaseStringUTFChars(file, pFile);

    auto engine = castHandle(handle);
    return engine->load(fileStr);
}

JNIEXPORT jint FK_JNI_METHOD_DEFINE(com_alimin_fk_engine, FkImageModel, nativeGetLayers)
        (JNIEnv *env, jobject that, jlong handle, jobject listener) {
    auto engine = castHandle(handle);
    auto lRef = std::make_shared<FkJniGlobalRef>(listener);
    auto callback = [lRef](std::shared_ptr<pb::FkPictureModel> &model) {
        FkJavaRuntime::getInstance().attachThread();
        JNIEnv *env = nullptr;
        if (FkJavaRuntime::getInstance().findEnv(&env)) {
            auto size = model->ByteSizeLong();
            std::vector<uint8_t> vec(size);
            memset(vec.data(), 0, size);
            auto buf = env->NewDirectByteBuffer(vec.data(), size);
            auto ret = model->SerializeToArray(vec.data(), vec.size());
            FkJavaFunc::makeNativeMsgListener(env, lRef->obj())->call(env, lRef->obj(), 0, ret, NULL, buf);
            env->DeleteLocalRef(buf);
        }
        FkJavaRuntime::getInstance().detachThread();
    };
    engine->getLayers(callback);
    return FK_OK;
}

#ifdef __cplusplus
}
#endif
