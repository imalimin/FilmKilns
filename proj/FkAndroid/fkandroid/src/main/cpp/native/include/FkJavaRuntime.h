/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FKANDROID_FkJAVARUNTIME_H
#define FKANDROID_FkJAVARUNTIME_H

#include "FkObject.h"
#include <jni.h>
#include <map>
#include <mutex>

//#define al_jni_interface_start(cls) \
//#ifdef __cplusplus \
//extern "C" { \
//#endif \
//#ifdef al_jni_class \
//#undef al_jni_class \
//#define al_jni_class Java_##cls \
//
//#define al_jni_interface_end \
//#ifdef al_jni_class \
//#undef al_jni_class \
//#ifdef __cplusplus \
//} \
//#endif \
//
//#define al_jni_interface_def(method) \
//##al_jni_class_##method \

///         Z: boolean
///         B: byte
///         C: char
///         S: short
///         I: int
///         J: long
///         F: float
///         D: double
/// Lpackage;: Objetc
///         V: void
FK_SUPER_CLASS(FkJavaRuntime, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkJavaRuntime)

public:
    static const int VERSION;

    static FkJavaRuntime &getInstance();

    static void jniThreadDestroy(void *envPtr);

public:
    FkJavaRuntime();

    ~FkJavaRuntime();

    void attach(JavaVM *vm);

    void detach();

    bool attachThread();

    void detachThread();

    bool findEnv(JNIEnv **env);

private:
    static FkJavaRuntime *instance;
    static pthread_key_t threadKey;
    JavaVM *jvm = nullptr;
    std::map<int64_t, JNIEnv *> mEnvMap;

    std::mutex atxMtx;
    std::mutex atxObjMtx;
};


#endif //FKANDROID_FkJAVARUNTIME_H
