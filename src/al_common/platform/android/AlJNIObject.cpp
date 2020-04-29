/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlJNIObject.h"

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
    auto itr = map.find(m.name);
    if (map.end() == itr) {
        jclass cls = env->GetObjectClass(o);
        if (cls) {
            *mid = env->GetMethodID(cls, m.name.c_str(), m.sign.c_str());
            return nullptr != *mid;
        }
    } else {
        *mid = itr->second;
        return true;
    }
    return false;
}

int AlJNIObject::callIntMethod(const Method &m, ...) {
    jmethodID mid;
    if (findMid(m, &mid)) {
        va_list args;
        va_start(args, m);
        int v = env->CallIntMethod(o, mid, args);
        va_end(args);
        env->ExceptionCheck();
        env->ExceptionClear();
        return v;
    }
    return 0;
}

void AlJNIObject::callVoidMethod(const AlJNIObject::Method &m, ...) {
    jmethodID mid;
    if (findMid(m, &mid)) {
        va_list args;
        va_start(args, m);
        env->CallVoidMethod(o, mid, args);
        va_end(args);
        env->ExceptionCheck();
        env->ExceptionClear();
    }
}

jobject AlJNIObject::callObjectMethod(const Method &m, ...) {
    jmethodID mid;
    if (findMid(m, &mid)) {
        va_list args;
        va_start(args, m);
        jobject obj = env->CallObjectMethod(o, mid, args);
        va_end(args);
        env->ExceptionCheck();
        env->ExceptionClear();
        return obj;
    }
    return nullptr;
}

AlBuffer *AlJNIObject::callBufferMethod(const Method &m, ...) {
    va_list args;
    va_start(args, m);
    jobject obj = callObjectMethod(m, args);
    va_end(args);
    if (nullptr == obj) {
        return nullptr;
    }

    jlong capacity = env->GetDirectBufferCapacity(obj);
    void *buf = env->GetDirectBufferAddress(obj);
    if (nullptr == buf || 0 == capacity) {
        return nullptr;
    }
    return AlBuffer::wrap(static_cast<uint8_t *>(buf), static_cast<size_t>(capacity));
}

std::vector<long> AlJNIObject::callLongArrayMethod(const Method &m, ...) {
    va_list args;
    va_start(args, m);
    jobject obj = callObjectMethod(m, args);
    va_end(args);

    std::vector<long> v;
    if (obj) {
        jlongArray jArray = static_cast<jlongArray>(obj);
        jsize len = env->GetArrayLength(jArray);
        jlong *array = env->GetLongArrayElements(jArray, 0);
        for (int i = 0; i < len; ++i) {
            v.emplace_back(array[i]);
        }
        env->ReleaseLongArrayElements(jArray, array, 0);
    }
    return v;
}
