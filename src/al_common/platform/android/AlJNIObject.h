/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALJNIOBJECT_H
#define HWVC_ANDROID_ALJNIOBJECT_H

#include "Object.h"
#include "AlBuffer.h"
#include "jni.h"
#include <map>
#include <vector>

#define al_jni_call_void(o, m, args...)                       \
jmethodID mid;                                                \
if (o->findMid(m, &mid)) {                                    \
    o->getEnv()->CallVoidMethod(o->getObject(), mid, ##args); \
    o->getEnv()->ExceptionCheck();                            \
    o->getEnv()->ExceptionClear();                            \
}                                                             \

#define al_jni_call_int(o, m, name, args...) \
jmethodID mid;                                                      \
if (o->findMid(m, &mid)) {                                          \
    name = o->getEnv()->CallIntMethod(o->getObject(), mid, ##args); \
    o->getEnv()->ExceptionCheck();                                  \
    o->getEnv()->ExceptionClear();                                  \
}                                                                   \

#define al_jni_call_object(o, m, name, args...) \
jmethodID mid; \
if (o->findMid(m, &mid)) { \
    name = o->getEnv()->CallObjectMethod(o->getObject(), mid, ##args); \
    o->getEnv()->ExceptionCheck(); \
    o->getEnv()->ExceptionClear(); \
} \

#define al_jni_call_buffer(o, m, name, args...) \
jobject o_##name = nullptr; \
al_jni_call_object(o, m, o_##name, ##args) \
if (o_##name) { \
    jlong capacity = o->getEnv()->GetDirectBufferCapacity(o_##name); \
    void *ptr = o->getEnv()->GetDirectBufferAddress(o_##name); \
    if (nullptr != ptr && 0 != capacity) { \
        name = AlBuffer::wrap(static_cast<uint8_t *>(ptr), static_cast<size_t>(capacity));\
    } \
} \


#define al_jni_call_long_array(o, m, name, args...) \
jobject o_##name = nullptr; \
al_jni_call_object(o, m, o_##name, ##args) \
if (o_##name) { \
    jlongArray jArray = (jlongArray)o_##name;\
    jsize len = o->getEnv()->GetArrayLength(jArray);\
    jlong *array = o->getEnv()->GetLongArrayElements(jArray, 0);\
    for (int i = 0; i < len; ++i) {\
        name.emplace_back(array[i]);\
    }\
    o->getEnv()->ReleaseLongArrayElements(jArray, array, 0);\
}\

#define al_jni_call_test(name) \
jobject o_##name; \

class AlJNIObjCollection;

al_def_class AlJNIObject al_extend Object {
public:
    typedef struct {
        string cls;
        string name;
        string sign;
    } Method;
public:
    friend class AlJNIObjCollection;

    AlJNIObject(JNIEnv *env, jobject o);

    AlJNIObject(const AlJNIObject &o);

    ~AlJNIObject();

    bool findMid(const Method &m, jmethodID *mid);

    JNIEnv *getEnv();

    jobject getObject();

private:
    JNIEnv *env = nullptr;
    jobject o = nullptr;
    std::map<std::string, jmethodID> map;
};


#endif //HWVC_ANDROID_ALJNIOBJECT_H
