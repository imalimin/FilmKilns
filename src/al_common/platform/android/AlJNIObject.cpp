/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlJNIObject.h"
#include "AlLogcat.h"

#define TAG "AlJNIObject"

AlJNIObject::AlJNIObject(JNIEnv *env, jobject o) : Object(), env(env), o(o) {
}

AlJNIObject::AlJNIObject(const AlJNIObject &o) : Object(), env(o.env), o(o.o) {
}

AlJNIObject::~AlJNIObject() {
    env = nullptr;
    o = nullptr;
    map.clear();
}

bool AlJNIObject::findMid(const AlJNIObject::Method &m, jmethodID *mid) {
    std::string key(m.cls);
    key.append("_");
    key.append(m.name);
    key.append("_");
    key.append(m.sign);
    auto itr = map.find(key);
    if (map.end() == itr) {
        jclass cls = env->GetObjectClass(o);
        *mid = nullptr;
        if (cls) {
            *mid = env->GetMethodID(cls, m.name.c_str(), m.sign.c_str());
            if (*mid) {
                map.insert({key, *mid});
            } else {
                AlLogE(TAG, "Cannot find %s", key.c_str());
            }
        }
        env->DeleteLocalRef(cls);
        env->ExceptionCheck();
        env->ExceptionClear();
        return nullptr != *mid;
    } else {
        *mid = itr->second;
        return true;
    }
}

JNIEnv *AlJNIObject::getEnv() {
    return env;
}

jobject AlJNIObject::getObject() {
    return o;
}
