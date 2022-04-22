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
        FkLogE(TAG, "detach failed.");
        return;
    }
}

bool FkJavaRuntime::attachThread() {
    if (!jvm) {
        FkLogE(TAG, "attachThread failed. Please call attach before.");
        return false;
    }
    int64_t id = FkThread::currentThreadId();
    FkAssert(0 != id, false);
    std::lock_guard<std::mutex> guard(atxMtx);
    JNIEnv *pEnv = nullptr;
    if (findEnv(&pEnv)) {
        FkLogE(TAG, "attachThread failed. Do not attach repeat. currentThreadId=%d", id);
        return false;
    }
    jvm->GetEnv(reinterpret_cast<void **>(&pEnv), VERSION);
    if (nullptr == pEnv) {
        int status = jvm->AttachCurrentThread(&pEnv, NULL);
        if (status < 0) {
            FkLogE(TAG, "attachThreadfailed.");
            return false;
        }
    }
    FkLogI(TAG, "attachThread currentThreadId=%p, env=%p", id, pEnv);
    mEnvMap.insert(std::pair<int64_t, JNIEnv *>(id, pEnv));
    return true;
}

void FkJavaRuntime::detachThread() {
    if (!jvm) {
        FkLogE(TAG, "detachThread failed. Please call attach before.");
        return;
    }
    std::lock_guard<std::mutex> guard(atxMtx);
    JNIEnv *pEnv = nullptr;
    if (findEnv(&pEnv)) {
        int64_t id = FkThread::currentThreadId();
        FkAssert(0 != id, );
        FkLogD(TAG, "currentThreadId=%p", id);
        int status = jvm->DetachCurrentThread();
        if (status < 0) {
            FkLogI(TAG, "detachThread failed. currentThreadId=%p. status=%d", id, status);
        }
        mEnvMap.erase(mEnvMap.find(id));
    } else {
        FkLogI(TAG, "detachThread failed. Env not found. currentThreadId=%p", FkThread::currentThreadId());
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
        return false;
    }
    *env = itr->second;
    return true;
}