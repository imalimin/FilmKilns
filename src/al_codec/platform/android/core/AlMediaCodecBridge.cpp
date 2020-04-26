/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlMediaCodecBridge.h"
#include "platform/android/AlJavaNativeHelper.h"

#define TAG "AlMediaCodecBridge"

#ifdef __cplusplus
extern "C" {
#endif

static JMethodDescription midCreate = {
        "com/lmy/hwvcnative/core/AlMediaCodec",
        "create", "()V"};

#ifdef __cplusplus
}
#endif

AlMediaCodecBridge::AlMediaCodecBridge() : Object() {
    jHandler = AlJavaNativeHelper::getInstance()->callStaticObjectMethod(midCreate);
    if (nullptr == jHandler) {
        AlLogI(TAG, "failed");
        return;
    }
    JNIEnv *env = nullptr;
    if (AlJavaNativeHelper::getInstance()->findEnv(&env)) {
        AlJavaNativeHelper::getInstance()->registerAnObject(env,
                                                            reinterpret_cast<jlong>(this),
                                                            jHandler);
    }
    AlLogI(TAG, "success");
}

AlMediaCodecBridge::~AlMediaCodecBridge() {
    JNIEnv *env = nullptr;
    if (AlJavaNativeHelper::getInstance()->findEnv(&env)) {
        AlJavaNativeHelper::getInstance()->unregisterAnObject(env, reinterpret_cast<jlong>(this));
    }
}
