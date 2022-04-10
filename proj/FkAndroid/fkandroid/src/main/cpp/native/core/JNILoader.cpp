/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/
#include "jni.h"
#include "FkJavaRuntime.h"

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    FkJavaRuntime::getInstance().attach(vm);
    return FkJavaRuntime::VERSION;
}

JNIEXPORT void JNI_OnUnload(JavaVM *vm, void *reserved) {
    FkJavaRuntime::getInstance().detach();
}

#ifdef __cplusplus
}
#endif