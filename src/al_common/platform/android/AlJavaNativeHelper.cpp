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
        Logcat::e("HWVC", "AlJavaNativeHelper::getAndroidApi failed.");
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
    Logcat::i("HWVC", "AlJavaNativeHelper::attach");
}

void AlJavaNativeHelper::detach() {
    Logcat::i("HWVC", "AlJavaNativeHelper::detach");
    if (!objMap.empty()) {
        Logcat::e("HWVC", "AlJavaNativeHelper::detach. %d jobject leak.", objMap.size());
        objMap.clear();
    }
    if (!envMap.empty()) {
        Logcat::e("HWVC", "AlJavaNativeHelper::detach. %d JNIEnv leak.", objMap.size());
        envMap.clear();
    }
    this->jvm = nullptr;
}

void AlJavaNativeHelper::registerAnObject(JNIEnv *env, jlong handler, jobject jHandler) {
    Logcat::i("HWVC", "AlJavaNativeHelper::registerAnObject(%p, %p)", handler, jHandler);
    if (objMap.end() == objMap.find(handler)) {
        objMap.insert(pair<jlong, jobject>(handler, env->NewGlobalRef(jHandler)));
    }
}

void AlJavaNativeHelper::unregisterAnObject(JNIEnv *env, jlong handler) {
    Logcat::i("HWVC", "AlJavaNativeHelper::unregisterAnObject(%p)", handler);
    auto itr = objMap.find(handler);
    if (objMap.end() != itr) {
        env->DeleteGlobalRef(itr->second);
        objMap.erase(itr);
    }
}

bool AlJavaNativeHelper::attachThread() {
    if (!jvm) {
        Logcat::e("HWVC", "AlJavaNativeHelper::attachThread failed. Please call attach before.");
        return false;
    }
    long id = Thread::currentThreadId();
    JNIEnv *pEnv = nullptr;
    if (findEnv(&pEnv)) {
        Logcat::e("HWVC", "AlJavaNativeHelper::attachThread(%p) failed. Do not attach repeat.", id);
        return false;
    }
//    int status = jvm->GetEnv(reinterpret_cast<void **>(&pEnv), JNI_VERSION_1_6);
    int status = jvm->AttachCurrentThread(&pEnv, NULL);
    if (status < 0) {
        Logcat::e("HWVC", "AlJavaNativeHelper::attachThread failed.");
        return false;
    }
    Logcat::i("HWVC", "AlJavaNativeHelper::attachThread(%p, %p)", id, pEnv);
    envMap.insert(pair<long, JNIEnv *>(id, pEnv));
    return true;
}

void AlJavaNativeHelper::detachThread() {
    if (!jvm) {
        Logcat::e("HWVC", "AlJavaNativeHelper::attachThread failed. Please call attach before.");
        return;
    }
    JNIEnv *pEnv = nullptr;
    if (findEnv(&pEnv)) {
        long id = Thread::currentThreadId();
        Logcat::i("HWVC", "AlJavaNativeHelper::detachThread(%p)", id);
        int status = jvm->DetachCurrentThread();
        if (status < 0) {
            Logcat::e("HWVC", "AlJavaNativeHelper::detachThread failed.");
        }
        envMap.erase(envMap.find(id));
    } else {
        long id = Thread::currentThreadId();
        Logcat::i("HWVC", "AlJavaNativeHelper::detachThread(%p) failed", id);
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
        jclass clazz = pEnv->FindClass(method.cls.c_str());
        if (nullptr == clazz) {
            AlLogE(TAG, "failed for %s", method.cls.c_str());
            return false;
        }
        jmethodID id = pEnv->GetStaticMethodID(clazz, method.name.c_str(), method.sign.c_str());
        pEnv->DeleteLocalRef(clazz);
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

bool AlJavaNativeHelper::callMethod(jlong handler, JMethodDescription method, ...) {
    va_list args;
    jobject jObject;
    JNIEnv *pEnv = nullptr;
    jmethodID methodID = nullptr;
    if (findEnv(&pEnv) &&
        findJObject(handler, &jObject) &&
        findMethod(handler, method, &methodID)) {
        va_start(args, methodID);
        pEnv->CallVoidMethod(jObject, methodID, args);
    }
    va_end(args);
    return true;
}

jobject AlJavaNativeHelper::callStaticObjectMethod(JMethodDescription method) {
    JNIEnv *pEnv = nullptr;
    jmethodID methodID = nullptr;
    std::string clsName("L");
    clsName.append(method.cls);
    clsName.append(";");
    if (findEnv(&pEnv) &&
        findStaticMethod(method, &methodID)) {
        jclass clazz = pEnv->FindClass(clsName.c_str());
        if (nullptr == clazz) {
            AlLogE(TAG, "failed");
            return nullptr;
        }
        auto obj = pEnv->CallStaticObjectMethod(clazz, methodID);
        if (pEnv->ExceptionCheck()) {
            AlLogE(TAG, "failed");
            return nullptr;
        }
        return obj;
    }
    AlLogE(TAG, "failed");
    return nullptr;
}
