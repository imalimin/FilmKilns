/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "gtest/gtest.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "jni.h"

#define TAG "FkModuleTest"

JNIEXPORT jboolean JNICALL Java_com_alimin_fk_CPPTest_testAll
        (JNIEnv *env, jobject thiz) {
    int argc = 0;
    char **argv = nullptr;
    ::testing::InitGoogleTest(&argc, argv);
    return 0 == RUN_ALL_TESTS() ? JNI_TRUE : JNI_FALSE;
}

#ifdef __cplusplus
}
#endif