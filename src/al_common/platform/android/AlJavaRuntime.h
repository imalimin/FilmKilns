/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALJAVARUNTIME_H
#define HWVC_ANDROID_ALJAVARUNTIME_H

#include "Object.h"
#include "AlJNIObjCollection.h"
#include <jni.h>
#include <log.h>
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
al_class AlJavaRuntime al_extend Object {
public:
    static AlJavaRuntime &getInstance();

public:
    AlJavaRuntime();

    ~AlJavaRuntime();

    void attach(JavaVM *vm);

    void detach();

    bool attachThread();

    void detachThread();

    jclass registerAnClass(const char *name);

    bool findEnv(JNIEnv **env);

    bool attach(Object *o, jobject j, bool reqGlobalRef = true);

    void detach(Object *o);

    bool findObj(Object *o, AlJNIObject **obj);

private:
    int64_t _currentId();

private:
    static AlJavaRuntime *instance;
    static const int VERSION;
    JavaVM *jvm = nullptr;
    std::map<string, jclass> mClassMap;
    AlJNIObjCollection *collection = nullptr;
    std::map<int64_t, JNIEnv *> mEnvMap;

    std::mutex atxMtx;
    std::mutex atxObjMtx;
};


#endif //HWVC_ANDROID_ALJAVARUNTIME_H
