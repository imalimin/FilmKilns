/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALJNIENV_H
#define HWVC_ANDROID_ALJNIENV_H

#include "Object.h"
#include "AlJNIObjCollection.h"
#include <jni.h>
#include <log.h>
#include <map>
#include <mutex>

al_class(AlJNIEnv) {
public:
    static AlJNIEnv &getInstance();

public:
    AlJNIEnv();

    ~AlJNIEnv();

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
    static AlJNIEnv *instance;
    static const int VERSION;
    JavaVM *jvm = nullptr;
    std::map<string, jclass> mClassMap;
    AlJNIObjCollection *collection = nullptr;
    std::map<int64_t, JNIEnv *> mEnvMap;

    std::mutex atxMtx;
    std::mutex atxObjMtx;
};


#endif //HWVC_ANDROID_ALJNIENV_H
