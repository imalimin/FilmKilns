/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkJavaRuntime.h"
#include "FkDefinition.h"
#include "FkThread.h"

#define TAG "FkJavaRuntime"

FkJavaRuntime *FkJavaRuntime::instance = new FkJavaRuntime();
const int FkJavaRuntime::VERSION = JNI_VERSION_1_6;

FkJavaRuntime &FkJavaRuntime::getInstance() {
    return *instance;
}

FkJavaRuntime::FkJavaRuntime() : FkObject() {
}

FkJavaRuntime::~FkJavaRuntime() {
}

void FkJavaRuntime::attach(JavaVM *vm) {
    this->jvm = vm;
}

void FkJavaRuntime::detach() {
    JNIEnv *env = nullptr;
    jvm->GetEnv(reinterpret_cast<void **>(&env), VERSION);
    this->jvm = nullptr;
    if (nullptr == env) {
        FkLogE(TAG, "failed.");
        return;
    }
}

bool FkJavaRuntime::attachThread() {
    if (!jvm) {
        FkLogE(TAG, "failed. Please call attach before.");
        return false;
    }
    int64_t id = FkThread::currentThreadId();
    FkAssert(0 != id, false);
    std::lock_guard<std::mutex> guard(atxMtx);
    JNIEnv *pEnv = nullptr;
    if (findEnv(&pEnv)) {
        FkLogE(TAG, "%p failed. Do not attach repeat.", id);
        return false;
    }
    jvm->GetEnv(reinterpret_cast<void **>(&pEnv), VERSION);
    if (nullptr == pEnv) {
        int status = jvm->AttachCurrentThread(&pEnv, NULL);
        if (status < 0) {
            FkLogE(TAG, "failed.");
            return false;
        }
    }
    FkLogI(TAG, "%p, %p", id, pEnv);
    mEnvMap.insert(std::pair<int64_t, JNIEnv *>(id, pEnv));
    return true;
}

void FkJavaRuntime::detachThread() {
    if (!jvm) {
        FkLogE(TAG, "failed. Please call attach before.");
        return;
    }
    std::lock_guard<std::mutex> guard(atxMtx);
    JNIEnv *pEnv = nullptr;
    if (findEnv(&pEnv)) {
        int64_t id = FkThread::currentThreadId();
        FkAssert(0 != id, );
        FkLogI(TAG, "%p", id);
        int status = jvm->DetachCurrentThread();
        if (status < 0) {
            FkLogI(TAG, "failed. Looper is %p", id);
        }
        mEnvMap.erase(mEnvMap.find(id));
    } else {
        int64_t id = FkThread::currentThreadId();
        FkLogI(TAG, "failed. Looper is %p", id);
    }

}

bool FkJavaRuntime::findEnv(JNIEnv **env) {
    int64_t id = FkThread::currentThreadId();
    FkAssert(0 != id, false);
    auto itr = mEnvMap.find(id);
    if (mEnvMap.end() == itr) {
        JNIEnv *pEnv = nullptr;
        jvm->GetEnv(reinterpret_cast<void **>(&pEnv), VERSION);
        if (nullptr != pEnv) {
            *env = pEnv;
            return true;
        }
        FkLogI(TAG, "failed. Looper is %p", id);
        return false;
    }
    *env = itr->second;
    return true;
}