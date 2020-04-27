/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "platform/android/AlJavaNativeHelper.h"
#include "FFUtils.h"

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    AlJavaNativeHelper::getInstance()->attach(vm);
    AlJavaNativeHelper::getInstance()->registerAnClass("com/lmy/hwvcnative/core/AlMediaCodec");
    FFUtils::attachJvm(vm);
    AlLogI("JNILoader", "AndroidApi: %d", AlJavaNativeHelper::getAndroidApi());
    return JNI_VERSION_1_6;
}

JNIEXPORT void JNI_OnUnload(JavaVM *vm, void *reserved) {
    AlJavaNativeHelper::getInstance()->detach();
}

#ifdef __cplusplus
}
#endif