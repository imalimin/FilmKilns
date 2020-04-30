/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlJNIEnv.h"
#include "Thread.h"
#include <cassert>

#define TAG "AlJNIEnv"

AlJNIEnv *AlJNIEnv::instance = new AlJNIEnv();
const int AlJNIEnv::VERSION = JNI_VERSION_1_6;

AlJNIEnv &AlJNIEnv::getInstance() {
    return *instance;
}

AlJNIEnv::AlJNIEnv() : Object() {

}

AlJNIEnv::~AlJNIEnv() = default;

void AlJNIEnv::attach(JavaVM *vm) {
    this->jvm = vm;
}

void AlJNIEnv::detach() {
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

bool AlJNIEnv::attachThread() {
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
    jvm->GetEnv(reinterpret_cast<void **>(&pEnv), JNI_VERSION_1_6);
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

void AlJNIEnv::detachThread() {
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

jclass AlJNIEnv::registerAnClass(const char *name) {
    auto itr = mClassMap.find(name);
    if (mClassMap.end() != itr) {
        AlLogW(TAG, "Repeat register.");
        return itr->second;
    }
    JNIEnv *env = nullptr;
    jvm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6);
    if (nullptr == env) {
        AlLogE(TAG, "failed.");
        return nullptr;
    }
    auto cls = env->FindClass(name);
    if (nullptr == cls) {
        AlLogE(TAG, "failed.");
        return nullptr;
    }
    mClassMap.insert(pair<std::string, jclass>(name,
                                               reinterpret_cast<jclass>(env->NewGlobalRef(cls))));
    return cls;
}

bool AlJNIEnv::findEnv(JNIEnv **env) {
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

bool AlJNIEnv::attach(Object *o, jobject j) {
    if (nullptr == o) {
        AlLogI(TAG, "failed");
        return false;
    }
    int64_t id = _currentId();
    assert(0 != id);
    std::lock_guard<std::mutex> guard(atxObjMtx);
    auto itr = map.find(id);
    AlJNIObjCollection *collection = nullptr;
    if (map.end() == itr) {
        collection = new AlJNIObjCollection();
        map.insert(std::pair<int64_t, AlJNIObjCollection *>(id, collection));
    } else {
        collection = itr->second;
    }
    JNIEnv *env = nullptr;
    if (!findEnv(&env)) {
        AlLogI(TAG, "failed");
        return false;
    }
    collection->attach(env, o, j);
    return true;
}

void AlJNIEnv::detach(Object *o) {
    int64_t id = _currentId();
    assert(0 != id);
    std::lock_guard<std::mutex> guard(atxObjMtx);
    auto itr = map.find(id);
    if (map.end() == itr) {
        AlLogI(TAG, "failed. Looper is %p", id);
        return;
    }
    auto *collection = itr->second;
    if (nullptr == collection) {
        AlLogI(TAG, "failed. Looper is %p", id);
        return;
    }
    JNIEnv *env = nullptr;
    if (!findEnv(&env)) {
        AlLogI(TAG, "failed");
        return;
    }
    collection->detach(env, o);
    if (0 == collection->size()) {
        map.erase(itr);
        delete collection;
    }
}

bool AlJNIEnv::findObj(Object *o, AlJNIObject **obj) {
    if (nullptr == o || nullptr == obj) {
        AlLogI(TAG, "failed");
        return false;
    }
    int64_t id = _currentId();
    assert(0 != id);
    std::lock_guard<std::mutex> guard(atxObjMtx);
    auto itr = map.find(id);
    if (map.end() == itr) {
        AlLogI(TAG, "failed. Looper is %p", id);
        return false;
    }
    auto *collection = itr->second;
    if (nullptr == collection) {
        AlLogI(TAG, "failed. Looper is %p", id);
        return false;
    }

    return collection->findObj(o, obj);
}

int64_t AlJNIEnv::_currentId() {
    return Thread::currentThreadId();
}