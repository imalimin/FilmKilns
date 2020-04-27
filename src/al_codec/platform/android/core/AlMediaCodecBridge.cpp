/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlMediaCodecBridge.h"
#include "platform/android/AlJavaNativeHelper.h"

#define TAG "AlMediaCodecBridge"

AlMediaCodecBridge::AlMediaCodecBridge() : Object() {
    JNIEnv *env = nullptr;
    if (!AlJavaNativeHelper::getInstance()->findEnv(&env)) {
        AlLogI(TAG, "failed");
        return;
    }
    jclass cls = AlJavaNativeHelper::getInstance()->registerAnClass("com/lmy/hwvcnative/core/AlMediaCodec");
    if (nullptr == cls) {
        AlLogI(TAG, "failed");
        return;
    }
    auto mid = env->GetMethodID(cls, "<init>", "()V");
    if (nullptr == mid) {
        AlLogI(TAG, "failed");
        return;
    }
    jHandler = env->NewObject(cls, mid);
    env->ExceptionCheck();
    env->ExceptionClear();
    if (nullptr == jHandler) {
        AlLogI(TAG, "failed");
        return;
    }
    AlJavaNativeHelper::getInstance()->registerAnObject(env,
                                                        reinterpret_cast<jlong>(this),
                                                        jHandler);
    AlLogI(TAG, "success");
}

AlMediaCodecBridge::~AlMediaCodecBridge() {
    JNIEnv *env = nullptr;
    if (AlJavaNativeHelper::getInstance()->findEnv(&env)) {
        AlJavaNativeHelper::getInstance()->unregisterAnObject(env, reinterpret_cast<jlong>(this));
    }
}
