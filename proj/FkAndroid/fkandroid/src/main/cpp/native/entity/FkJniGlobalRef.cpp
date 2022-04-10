//
// Created by Alimin on 2022/4/10.
//

#include "FkJniGlobalRef.h"
#include "FkJavaRuntime.h"

FkJniGlobalRef::FkJniGlobalRef(jobject obj) : FkObject() {
    JNIEnv *env = nullptr;
    if (FkJavaRuntime::getInstance().findEnv(&env)) {
        _obj = env->NewGlobalRef(obj);
    }
}

FkJniGlobalRef::~FkJniGlobalRef() {
    JNIEnv *env = nullptr;
    if (FkJavaRuntime::getInstance().findEnv(&env)) {
        env->DeleteGlobalRef(_obj);
    }
}

jobject FkJniGlobalRef::obj() {
    return _obj;
}
