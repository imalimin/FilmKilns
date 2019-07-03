/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwJavaNativeHelper.h"
#include "Thread.h"
#include "StringUtils.h"
#include <sys/system_properties.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "ff/libavcodec/jni.h"

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    HwJavaNativeHelper::getInstance()->attach(vm);
    av_jni_set_java_vm(vm, NULL);
    Logcat::i("HWVC", "HwJavaNativeHelper::getAndroidApi %d", HwJavaNativeHelper::getAndroidApi());
    return JNI_VERSION_1_6;
}

JNIEXPORT void JNI_OnUnload(JavaVM *vm, void *reserved) {
    HwJavaNativeHelper::getInstance()->detach();
}

#ifdef __cplusplus
}
#endif

string HwJavaNativeHelper::getClassName(JNIEnv *env, jobject object) {
    jclass cls = env->FindClass("java/lang/Class");
    jmethodID mid_getName = env->GetMethodID(cls, "getName", "()Ljava/lang/String;");
    jstring name = static_cast<jstring>(env->CallObjectMethod(object, mid_getName));
    const char *pName = env->GetStringUTFChars(name, JNI_FALSE);
    std::string nameStr(pName);
    env->ReleaseStringUTFChars(name, pName);
    env->DeleteLocalRef(cls);
    return nameStr;
}

int HwJavaNativeHelper::getAndroidApi() {
    string key = "ro.build.version.sdk";
    char value[128] = {0};
    int ret = __system_property_get(key.c_str(), value);
    if (ret <= 0) {
        Logcat::e("HWVC", "HwJavaNativeHelper::getAndroidApi failed.");
        return 0;
    }
    return atoi(value);
}

HwJavaNativeHelper *HwJavaNativeHelper::instance = new HwJavaNativeHelper();

HwJavaNativeHelper *HwJavaNativeHelper::getInstance() {
    return instance;
}

HwJavaNativeHelper::HwJavaNativeHelper() : Object() {

}

HwJavaNativeHelper::HwJavaNativeHelper(const HwJavaNativeHelper &object) {

}

HwJavaNativeHelper::~HwJavaNativeHelper() {

}

HwJavaNativeHelper &HwJavaNativeHelper::operator=(const HwJavaNativeHelper &object) {

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
    long id = Thread::currentThreadId();
    JNIEnv *pEnv = nullptr;
    if (findEnv(&pEnv)) {
        Logcat::e("HWVC", "HwJavaNativeHelper::attachThread(%p) failed. Do not attach repeat.", id);
        return false;
    }
//    int status = jvm->GetEnv(reinterpret_cast<void **>(&pEnv), JNI_VERSION_1_6);
    int status = jvm->AttachCurrentThread(&pEnv, NULL);
    if (status < 0) {
        Logcat::e("HWVC", "HwJavaNativeHelper::attachThread failed.");
        return false;
    }
    Logcat::i("HWVC", "HwJavaNativeHelper::attachThread(%p, %p)", id, pEnv);
    envMap.insert(pair<long, JNIEnv *>(id, pEnv));
    return true;
}

void HwJavaNativeHelper::detachThread() {
    if (!jvm) {
        Logcat::e("HWVC", "HwJavaNativeHelper::attachThread failed. Please call attach before.");
        return;
    }
    JNIEnv *pEnv = nullptr;
    if (findEnv(&pEnv)) {
        long id = Thread::currentThreadId();
        Logcat::i("HWVC", "HwJavaNativeHelper::detachThread(%p)", id);
        int status = jvm->DetachCurrentThread();
        if (status < 0) {
            Logcat::e("HWVC", "HwJavaNativeHelper::detachThread failed.");
        }
        envMap.erase(envMap.find(id));
    } else {
        long id = Thread::currentThreadId();
        Logcat::i("HWVC", "HwJavaNativeHelper::detachThread(%p) failed", id);
    }
}

bool HwJavaNativeHelper::findEnv(JNIEnv **env) {
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
        *methodID = pEnv->GetMethodID(clazz, method.name.c_str(), method.sign.c_str());
        pEnv->DeleteLocalRef(clazz);
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