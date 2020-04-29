/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlJavaNativeHelper.h"
#include "Thread.h"
#include "StringUtils.h"
#include "AlString.h"
#include <sys/system_properties.h>
#include <assert.h>

#define TAG "AlJavaNativeHelper"

string AlJavaNativeHelper::getClassName(JNIEnv *env, jobject object) {
    jclass cls = env->FindClass("java/lang/Class");
    jmethodID mid_getName = env->GetMethodID(cls, "getName", "()Ljava/lang/String;");
    jstring name = static_cast<jstring>(env->CallObjectMethod(object, mid_getName));
    const char *pName = env->GetStringUTFChars(name, JNI_FALSE);
    std::string nameStr(pName);
    env->ReleaseStringUTFChars(name, pName);
    env->DeleteLocalRef(cls);
    return nameStr;
}

int AlJavaNativeHelper::getAndroidApi() {
    string key = "ro.build.version.sdk";
    char value[128] = {0};
    int ret = __system_property_get(key.c_str(), value);
    if (ret <= 0) {
        AlLogE(TAG, "failed.");
        return 0;
    }
    return atoi(value);
}

AlJavaNativeHelper *AlJavaNativeHelper::instance = new AlJavaNativeHelper();

AlJavaNativeHelper *AlJavaNativeHelper::getInstance() {
    return instance;
}

AlJavaNativeHelper::AlJavaNativeHelper() : Object() {

}

AlJavaNativeHelper::AlJavaNativeHelper(const AlJavaNativeHelper &object) {

}

AlJavaNativeHelper::~AlJavaNativeHelper() {

}

AlJavaNativeHelper &AlJavaNativeHelper::operator=(const AlJavaNativeHelper &object) {

}


void AlJavaNativeHelper::attach(JavaVM *vm) {
    this->jvm = vm;
    AlLogI(TAG, "success.");
}

void AlJavaNativeHelper::detach() {
    JNIEnv *env = nullptr;
    jvm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6);
    this->jvm = nullptr;
    if (nullptr == env) {
        AlLogE(TAG, "failed.");
        return;
    }
    for (auto &itr : objMap) {
        AlLogI(TAG, "%d jobject leak. Release now.", objMap.size());
        env->DeleteGlobalRef(itr.second);
    }
    objMap.clear();
    methodMap.clear();
    sMethodMap.clear();
    for (auto &itr : mClassMap) {
        AlLogI(TAG, "%d jclass leak. Release now.", mClassMap.size());
        env->DeleteGlobalRef(itr.second);
    }
    mClassMap.clear();
    if (!envMap.empty()) {
        AlLogI(TAG, "%d JNIEnv leak. Release now.", envMap.size());
        envMap.clear();
    }
}

jclass AlJavaNativeHelper::registerAnClass(const char *name) {
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

void AlJavaNativeHelper::registerAnObject(JNIEnv *env, jlong handler, jobject jHandler) {
    AlLogI(TAG, "%p, %p", handler, jHandler);
    assert(0 != handler);
    if (objMap.end() == objMap.find(handler)) {
        objMap.insert(pair<jlong, jobject>(handler, env->NewGlobalRef(jHandler)));
    }
}

void AlJavaNativeHelper::unregisterAnObject(JNIEnv *env, jlong handler) {
    AlLogI(TAG, "%p", handler);
    auto itr = objMap.find(handler);
    if (objMap.end() != itr) {
        env->DeleteGlobalRef(itr->second);
        objMap.erase(itr);
    }
}

bool AlJavaNativeHelper::attachThread() {
    if (!jvm) {
        AlLogE(TAG, "failed. Please call attach before.");
        return false;
    }
    long id = Thread::currentThreadId();
    JNIEnv *pEnv = nullptr;
    if (findEnv(&pEnv)) {
        AlLogE(TAG, "%p failed. Do not attach repeat.", id);
        return false;
    }
//    int status = jvm->GetEnv(reinterpret_cast<void **>(&pEnv), JNI_VERSION_1_6);
    int status = jvm->AttachCurrentThread(&pEnv, NULL);
    if (status < 0) {
        AlLogE(TAG, "failed.");
        return false;
    }
    AlLogI(TAG, "%p, %p", id, pEnv);
    envMap.insert(pair<long, JNIEnv *>(id, pEnv));
    return true;
}

void AlJavaNativeHelper::detachThread() {
    if (!jvm) {
        AlLogE(TAG, "failed. Please call attach before.");
        return;
    }
    JNIEnv *pEnv = nullptr;
    if (findEnv(&pEnv)) {
        long id = Thread::currentThreadId();
        AlLogI(TAG, "%p", id);
        int status = jvm->DetachCurrentThread();
        if (status < 0) {
            AlLogE(TAG, "failed.");
        }
        envMap.erase(envMap.find(id));
    } else {
        long id = Thread::currentThreadId();
        AlLogI(TAG, "%p failed", id);
    }
}

bool AlJavaNativeHelper::findEnv(JNIEnv **env) {
//    int status = jvm->GetEnv(reinterpret_cast<void **>(env), JNI_VERSION_1_6);
//    if (status >= 0) {
//        return true;
//    }
    auto itr = envMap.find(Thread::currentThreadId());
    if (envMap.end() == itr) {
        *env = nullptr;
        return false;
    }
    *env = itr->second;
    return true;
}

bool AlJavaNativeHelper::findJObject(jlong handler, jobject *jObject) {
    auto itr = objMap.find(handler);
    if (objMap.end() == itr) {
        *jObject = nullptr;
        return false;
    }
    *jObject = itr->second;
    return true;
}

bool AlJavaNativeHelper::findStaticMethod(JMethodDescription method, jmethodID *methodID) {
    JNIEnv *pEnv = nullptr;
    if (!findEnv(&pEnv)) {
        AlLogE(TAG, "failed");
        return false;
    }
    string key = method.cls + method.name + method.sign;
    auto itr = sMethodMap.find(key);
    if (sMethodMap.end() == itr) {
        AlLogI(TAG, "%s", method.cls.c_str());
        jclass clazz = registerAnClass(method.cls.c_str());
        if (nullptr == clazz) {
            AlLogE(TAG, "failed for %s", method.cls.c_str());
            return false;
        }
        jmethodID id = pEnv->GetStaticMethodID(clazz, method.name.c_str(), method.sign.c_str());
        if (!id || pEnv->ExceptionCheck()) {
            *methodID = nullptr;
            AlLogE(TAG, "failed");
            return false;
        }
        *methodID = id;
        sMethodMap.insert(pair<string, jmethodID>(key, *methodID));
    } else {
        *methodID = itr->second;
    }
    return true;
}

bool AlJavaNativeHelper::findMethod(jlong handler, JMethodDescription method, jmethodID *methodID) {
    jobject jObject = nullptr;
    JNIEnv *pEnv = nullptr;
    if (!findEnv(&pEnv)) {
        return false;
    }
    if (!findJObject(handler, &jObject)) {
        return false;
    }
    string key = method.cls + method.name + method.sign;
    auto itr = methodMap.find(key);
    if (methodMap.end() == itr) {
        jclass clazz = pEnv->GetObjectClass(jObject);
        jmethodID id = pEnv->GetMethodID(clazz, method.name.c_str(), method.sign.c_str());
        pEnv->DeleteLocalRef(clazz);
        if (!id || pEnv->ExceptionCheck()) {
            *methodID = nullptr;
            return false;
        }
        *methodID = id;
        methodMap.insert(pair<string, jmethodID>(key, *methodID));
    } else {
        *methodID = itr->second;
    }
    return true;
}
