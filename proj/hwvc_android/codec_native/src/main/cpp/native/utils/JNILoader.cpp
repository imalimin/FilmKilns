/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include <include/HwResult.h>
#include "platform/android/AlJavaNativeHelper.h"
#include "platform/android/AlJavaRuntime.h"
#include "AlFFUtils.h"

static std::vector<std::string> JAVA_CLASS_SET =
        {"com/lmy/hwvcnative/core/AlMediaCodec",
         "com/lmy/hwvcnative/core/AlMediaCodecKt"};

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jint JNICALL Java_com_lmy_hwvcnative_tools_AlJavaRuntime_init
        (JNIEnv *env, jobject thiz) {
    JavaVM *vm = nullptr;
    env->GetJavaVM(&vm);
    if (nullptr == vm) {
        return Hw::FAILED.code;
    }
    AlJavaRuntime::getInstance().attach(vm);
    AlJavaNativeHelper::getInstance()->attach(vm);
    for (auto cls : JAVA_CLASS_SET) {
        AlJavaRuntime::getInstance().registerAnClass(cls.c_str());
        AlJavaNativeHelper::getInstance()->registerAnClass(cls.c_str());

    }
    AlFFUtils::attachJvm(vm);
    AlLogI("JNILoader", "AndroidApi: %d", AlJavaNativeHelper::getAndroidApi());
    return Hw::OK.code;
}

JNIEXPORT void JNI_OnUnload(JavaVM *vm, void *reserved) {
    AlJavaNativeHelper::getInstance()->detach();
    AlJavaRuntime::getInstance().detach();
}

#ifdef __cplusplus
}
#endif