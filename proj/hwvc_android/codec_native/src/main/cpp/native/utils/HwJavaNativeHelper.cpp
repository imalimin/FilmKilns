/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwJavaNativeHelper.h"

HwJavaNativeHelper *HwJavaNativeHelper::create() {
    return new HwJavaNativeHelper();
}

HwJavaNativeHelper::HwJavaNativeHelper() : Object() {

}

HwJavaNativeHelper::~HwJavaNativeHelper() {

}

void HwJavaNativeHelper::attach(JavaVM *vm) {
    this->jvm = vm;
    Logcat::i("HWVC", "HwJavaNativeHelper::attach");
}

void HwJavaNativeHelper::detach() {
    Logcat::i("HWVC", "HwJavaNativeHelper::detach");
    if (!objMap.empty()) {
        Logcat::e("HWVC", "HwJavaNativeHelper::detach. %d jobject leak.", objMap.size());
        objMap.clear();
    }
    if (!envMap.empty()) {
        Logcat::e("HWVC", "HwJavaNativeHelper::detach. %d JNIEnv leak.", objMap.size());
        envMap.clear();
    }
    this->jvm = nullptr;
}

void HwJavaNativeHelper::registerAnObject(JNIEnv *env, jlong handler, jobject jHandler) {
    Logcat::i("HWVC", "HwJavaNativeHelper::registerAnObject(%p, %p)", handler, jHandler);
    if (objMap.end() == objMap.find(handler)) {
        objMap.insert(pair<jlong, jobject>(handler, env->NewGlobalRef(jHandler)));
    }
}

void HwJavaNativeHelper::unregisterAnObject(JNIEnv *env, jlong handler) {
    Logcat::i("HWVC", "HwJavaNativeHelper::unregisterAnObject(%p)", handler);
    auto itr = objMap.find(handler);
    if (objMap.end() != itr) {
        env->DeleteGlobalRef(itr->second);
        objMap.erase(itr);
    }
}

bool HwJavaNativeHelper::attachThread() {
    if (!jvm) {
        Logcat::e("HWVC", "HwJavaNativeHelper::attachThread failed. Please call attach before.");
        return false;
    }
    JNIEnv *pEnv = nullptr;
    if (findEnv(&pEnv)) {
        Logcat::e("HWVC", "HwJavaNativeHelper::attachThread failed. Do not attach repeat.");
        return false;
    }
    int status = jvm->AttachCurrentThread(&pEnv, NULL);
    if (status < 0) {
        Logcat::e("HWVC", "HwJavaNativeHelper::attachThread failed.");
        return false;
    }
    long id = pthread_self();
    Logcat::i("HWVC", "HwJavaNativeHelper::attachThread(%p, %p)", id, pEnv);
    envMap.insert(pair<jlong, JNIEnv *>(id, pEnv));
    return true;
}

void HwJavaNativeHelper::detachThread() {
    if (!jvm) {
        Logcat::e("HWVC", "HwJavaNativeHelper::attachThread failed. Please call attach before.");
        return;
    }
    JNIEnv *pEnv = nullptr;
    if (findEnv(&pEnv)) {
        long id = pthread_self();
        Logcat::i("HWVC", "HwJavaNativeHelper::detachThread(%p)", id);
        jvm->DetachCurrentThread();
        envMap.erase(envMap.find(id));
    } else {
        long id = pthread_self();
        Logcat::i("HWVC", "HwJavaNativeHelper::detachThread(%p) failed", id);
    }
}

bool HwJavaNativeHelper::findEnv(JNIEnv **env) {
    auto itr = envMap.find(pthread_self());
    if (envMap.end() == itr) {
        *env = nullptr;
        return false;
    }
    *env = itr->second;
    return true;
}

bool HwJavaNativeHelper::findJObject(jlong handler, jobject *jObject) {
    auto itr = objMap.find(handler);
    if (objMap.end() == itr) {
        *jObject = nullptr;
        return false;
    }
    *jObject = itr->second;
    return true;
}

bool HwJavaNativeHelper::findMethod(jlong handler, JMethodDescription method, jmethodID *methodID) {
    jobject jObject;
    JNIEnv *pEnv = nullptr;
    if (!findEnv(&pEnv)) {
        return true;
    }
    if (!findJObject(handler, &jObject)) {
        return true;
    }
    string key = method.name + method.sign;
    auto itr = methodMap.find(key);
    if (methodMap.end() == itr) {
        jclass clazz = pEnv->GetObjectClass(jObject);
        *methodID = pEnv->GetMethodID(clazz, method.name.c_str(), method.sign.c_str());
        methodMap.insert(pair<string, jmethodID>(key, *methodID));
    } else {
        *methodID = itr->second;
    }
    return true;
}

bool HwJavaNativeHelper::callMethod(jlong handler, JMethodDescription method, ...) {
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