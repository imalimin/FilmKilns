/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlLogcat.h"
//#include "FkClassTypeTest.h"
//#include "gtest/gtest.h"
//
//TEST_F(FkClassTypeTest, Euqals) {
//    EXPECT_TRUE(FkClassTypeEquals());
//}

#ifdef __cplusplus
extern "C" {
#endif

#include "jni.h"

#define TAG "FkModuleTest"

JNIEXPORT void JNICALL Java_com_alimin_fk_module_FkModuleTest_testAll
        (JNIEnv *env, jobject thiz) {
//    int argc = 0;
//    char **argv = nullptr;
//    ::testing::InitGoogleTest(&argc, argv);
//    RUN_ALL_TESTS();
}

JNIEXPORT void JNICALL Java_com_alimin_fk_module_FkModuleTest_testClassType
        (JNIEnv *env, jobject thiz) {

}

#ifdef __cplusplus
}
#endif