/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "platform/android/AlJNIEnv.h"
#include "platform/android/AlJNIObject.h"
#include "AlFFUtils.h"

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jint JNICALL Java_com_lmy_hwvcnative_tools_AlFFUtils_exec(JNIEnv *env,
                                                                    jobject thiz,
                                                                    jstring cmd) {
    const char *pCmd = env->GetStringUTFChars(cmd, JNI_FALSE);
    std::string str = std::string(pCmd);
    env->ReleaseStringChars(cmd, reinterpret_cast<const jchar *>(pCmd));
    return AlFFUtils::exec(str);
}

#ifdef __cplusplus
}
#endif