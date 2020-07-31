/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlJavaRuntime.h"
#include "Thread.h"
#include <cassert>

#define TAG "AlJavaRuntime"

AlJavaRuntime *AlJavaRuntime::instance = new AlJavaRuntime();
const int AlJavaRuntime::VERSION = JNI_VERSION_1_6;

AlJavaRuntime &AlJavaRuntime::getInstance() {
    return *instance;
}

AlJavaRuntime::AlJavaRuntime() : Object() {
    collection = new AlJNIObjCollection();
}

AlJavaRuntime::~AlJavaRuntime() {
    delete collection;
    collection = nullptr;
}

void AlJavaRuntime::attach(JavaVM *vm) {
    this->jvm = vm;
}

void AlJavaRuntime::detach() {
    JNIEnv *env = nullptr;
    jvm->GetEnv(reinterpret_cast<void **>(&env), VERSION);
    this->jvm = nullptr;
    if (nullptr == env) {
        AlLogE(TAG, "failed.");
        return;
    }
    for (auto &itr : mClassMap) {
        AlLogI(TAG, "%d jclass leak. Release now.", mClassMap.size());
        env->DeleteGlobalRef(itr.second);
    }
    mClassMap.clear();
}

bool AlJavaRuntime::attachThread() {
    if (!jvm) {
        AlLogE(TAG, "failed. Please call attach before.");
        return false;
    }
    int64_t id = _currentId();
    assert(0 != id);
    std::lock_guard<std::mutex> guard(atxMtx);
    JNIEnv *pEnv = nullptr;
    if (findEnv(&pEnv)) {
        AlLogE(TAG, "%p failed. Do not attach repeat.", id);
        return false;
    }
    jvm->GetEnv(reinterpret_cast<void **>(&pEnv), VERSION);
    if (nullptr == pEnv) {
        int status = jvm->AttachCurrentThread(&pEnv, NULL);
        if (status < 0) {
            AlLogE(TAG, "failed.");
            return false;
        }
    }
    AlLogI(TAG, "%p, %p", id, pEnv);
    mEnvMap.insert(pair<int64_t, JNIEnv *>(id, pEnv));
    return true;
}

void AlJavaRuntime::detachThread() {
    if (!jvm) {
        AlLogE(TAG, "failed. Please call attach before.");
        return;
    }
    std::lock_guard<std::mutex> guard(atxMtx);
    JNIEnv *pEnv = nullptr;
    if (findEnv(&pEnv)) {
        int64_t id = _currentId();
        assert(0 != id);
        AlLogI(TAG, "%p", id);
        int status = jvm->DetachCurrentThread();
        if (status < 0) {
            AlLogI(TAG, "failed. Looper is %p", id);
        }
        mEnvMap.erase(mEnvMap.find(id));
    } else {
        int64_t id = _currentId();
        AlLogI(TAG, "failed. Looper is %p", id);
    }

}

jclass AlJavaRuntime::registerAnClass(const char *name) {
    auto itr = mClassMap.find(name);
    if (mClassMap.end() != itr) {
        AlLogW(TAG, "Repeat register.");
        return itr->second;
    }
    JNIEnv *env = nullptr;
    jvm->GetEnv(reinterpret_cast<void **>(&env), VERSION);
    if (nullptr == env) {
        AlLogE(TAG, "failed. Register on JNI_ONLoad pls.");
        return nullptr;
    }
    auto cls = env->FindClass(name);
    if (nullptr == cls) {
        AlLogE(TAG, "failed.");
        return nullptr;
    }
    mClassMap.insert({name, (jclass) env->NewGlobalRef(cls)});
    return cls;
}

bool AlJavaRuntime::findEnv(JNIEnv **env) {
    int64_t id = _currentId();
    assert(0 != id);
    auto itr = mEnvMap.find(id);
    if (mEnvMap.end() == itr) {
        AlLogI(TAG, "failed. Looper is %p", id);
        return false;
    }
    *env = itr->second;
    return true;
}

bool AlJavaRuntime::attach(Object *o, jobject j, bool reqGlobalRef) {
    if (nullptr == o) {
        AlLogI(TAG, "failed");
        return false;
    }
    std::lock_guard<std::mutex> guard(atxObjMtx);
    JNIEnv *env = nullptr;
    if (!findEnv(&env)) {
        AlLogI(TAG, "failed");
        return false;
    }
    collection->attach(env, o, j, reqGlobalRef);
    return true;
}

void AlJavaRuntime::detach(Object *o) {
    std::lock_guard<std::mutex> guard(atxObjMtx);
    JNIEnv *env = nullptr;
    if (!findEnv(&env)) {
        AlLogI(TAG, "failed");
        return;
    }
    collection->detach(env, o);
}

bool AlJavaRuntime::findObj(Object *o, AlJNIObject **obj) {
    if (nullptr == o || nullptr == obj) {
        AlLogI(TAG, "failed");
        return false;
    }
    std::lock_guard<std::mutex> guard(atxObjMtx);
    return collection->findObj(o, obj);
}

int64_t AlJavaRuntime::_currentId() {
    return Thread::currentThreadId();
}