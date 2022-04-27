/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "jni.h"
#include "FkImageEngine.h"
#include "FkGraphicWindow.h"
#include "FkRenderEngine.h"
#include "FkInstanceHolder.h"
#include "FkAndroidWindow.h"

#define RENDER_ALIAS "RenderEngine"
#define IMAGE_ENGINE_ALIAS "ImageEngine"

#ifdef __cplusplus
extern "C" {
#endif

static std::shared_ptr<FkImageEngine> castHandle(jlong handle) {
    return  FkInstanceHolder::getInstance().find<std::shared_ptr<FkImageEngine>>(handle);
}

JNIEXPORT jlong JNICALL Java_com_alimin_fk_engine_FkImage_nativeCreateInstance
        (JNIEnv *env, jobject that, jstring workspace) {
    auto pWorkspace = env->GetStringUTFChars(workspace, nullptr);
    std::string workspaceStr(pWorkspace);
    env->ReleaseStringUTFChars(workspace, pWorkspace);
    std::shared_ptr<FkEngine> renderEngine = std::make_shared<FkRenderEngine>(RENDER_ALIAS);
    auto imageEngine = std::make_shared<FkImageEngine>(renderEngine, workspaceStr, IMAGE_ENGINE_ALIAS);
    return FkInstanceHolder::getInstance().put(imageEngine);
}

JNIEXPORT void JNICALL Java_com_alimin_fk_engine_FkImage_nativeCreate
        (JNIEnv *env, jobject that, jlong handle) {
    auto engine = castHandle(handle);
    engine->create();
}

JNIEXPORT void JNICALL Java_com_alimin_fk_engine_FkImage_nativeDestroy
        (JNIEnv *env, jobject that, jlong handle) {
    auto engine = castHandle(handle);
    engine->destroy();
    FkInstanceHolder::getInstance().release(handle);
}

JNIEXPORT void JNICALL Java_com_alimin_fk_engine_FkImage_nativeStart
        (JNIEnv *env, jobject that, jlong handle) {
    auto engine = castHandle(handle);
    engine->start();
}

JNIEXPORT void JNICALL Java_com_alimin_fk_engine_FkImage_nativeStop
        (JNIEnv *env, jobject that, jlong handle) {
    auto engine = castHandle(handle);
    engine->stop();
}

JNIEXPORT jint JNICALL Java_com_alimin_fk_engine_FkImage_nativeSetSurface
        (JNIEnv *env, jobject that, jlong handle, jobject surface, jint scaleType) {
    auto engine = castHandle(handle);
    if (surface) {
        auto win = std::make_shared<FkAndroidWindow>(surface);
        return engine->setSurface(win, scaleType);
    } else {
        return engine->setSurface(nullptr, scaleType);
    }
}

JNIEXPORT jint JNICALL Java_com_alimin_fk_engine_FkImage_nativeNewLayerWithFile
        (JNIEnv *env, jobject that, jlong handle, jstring path) {
    auto engine = castHandle(handle);
    auto *p = env->GetStringUTFChars(path, nullptr);
    auto layer = engine->newLayerWithFile(std::string(p));
    env->ReleaseStringUTFChars(path, p);
    return layer;
}

JNIEXPORT jint JNICALL Java_com_alimin_fk_engine_FkImage_nativeNewLayerWithColor
        (JNIEnv *env, jobject that, jlong handle, jint widht, jint height, jint red, jint green,
         jint blue, jint alpha) {
    FkColor color = FkColor::makeFromRGBA8(red, green, blue, alpha);
    color.setAlphaType(FkColor::AlphaType::kPreMultiple);
    auto engine = castHandle(handle);
    return engine->newLayerWithColor(FkSize(widht, height), color);
}

JNIEXPORT jint JNICALL Java_com_alimin_fk_engine_FkImage_nativeRemoveLayer
        (JNIEnv *env, jobject that, jlong handle, jint layerId) {
    auto engine = castHandle(handle);
    return engine->removeLayer(layerId);
}

JNIEXPORT jint JNICALL Java_com_alimin_fk_engine_FkImage_nativeNotifyRender
        (JNIEnv *env, jobject that, jlong handle) {
    auto engine = castHandle(handle);
    return engine->notifyRender();
}

JNIEXPORT jint JNICALL Java_com_alimin_fk_engine_FkImage_nativeSetCanvasSize
        (JNIEnv *env, jobject that, jlong handle, jint width, jint height) {
    auto engine = castHandle(handle);
    return engine->setCanvasSize(FkSize(width, height));
}

JNIEXPORT jint JNICALL Java_com_alimin_fk_engine_FkImage_nativePostTranslate
        (JNIEnv *env, jobject that, jlong handle, jint layer, jint dx, jint dy) {
    auto engine = castHandle(handle);
    return engine->postTranslate(layer, dx, dy);
}

JNIEXPORT jint JNICALL Java_com_alimin_fk_engine_FkImage_nativePostScale
        (JNIEnv *env, jobject that, jlong handle, jint layer, jfloat dx, jfloat dy) {
    auto engine = castHandle(handle);
    return engine->postScale(layer, dx, dy);
}

JNIEXPORT jint JNICALL Java_com_alimin_fk_engine_FkImage_nativePostRotation
        (JNIEnv *env, jobject that, jlong handle, jint layer, jint num, jint den) {
    auto engine = castHandle(handle);
    FkRational rational(num, den);
    return engine->postRotation(layer, rational);
}

JNIEXPORT jint JNICALL Java_com_alimin_fk_engine_FkImage_nativeDrawPoint
        (JNIEnv *env, jobject that, jlong handle, jint layer, jlong color, jint size, jint x, jint y) {
    auto engine = castHandle(handle);
    return engine->drawPoint(layer, FkColor::makeFrom(color), size, x, y);
}

JNIEXPORT jint JNICALL Java_com_alimin_fk_engine_FkImage_nativeCrop
        (JNIEnv *env, jobject that, jlong handle, jint left, jint top, jint right, jint bottom) {
    auto engine = castHandle(handle);
    FkIntRect rect(left, top, right, bottom);
    return engine->crop(rect);
}

JNIEXPORT jint JNICALL Java_com_alimin_fk_engine_FkImage_nativeCropLayer
        (JNIEnv *env, jobject that, jlong handle, jint layer, jint left, jint top, jint right, jint bottom) {
    auto engine = castHandle(handle);
    FkIntRect rect(left, top, right, bottom);
    return engine->cropLayer(layer, rect);
}

JNIEXPORT jint JNICALL Java_com_alimin_fk_engine_FkImage_nativeSave
        (JNIEnv *env, jobject that, jlong handle, jstring file) {
    auto engine = castHandle(handle);
    auto *p = env->GetStringUTFChars(file, nullptr);
    auto ret = engine->save(std::string(p));
    env->ReleaseStringUTFChars(file, p);
    return ret;
}

#ifdef __cplusplus
}
#endif