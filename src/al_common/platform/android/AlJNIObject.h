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

class AlJNIObjCollection;

al_def_class(AlJNIObject) {
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

    int callIntMethod(const Method &m, ...);

    void callVoidMethod(const Method &m, ...);

    jobject callObjectMethod(const Method &m, ...);

    AlBuffer *callBufferMethod(const Method &m, ...);

    std::vector<long> callLongArrayMethod(const Method &m, ...);

private:
    JNIEnv *env = nullptr;
    jobject o = nullptr;
    std::map<std::string, jmethodID> map;
};


#endif //HWVC_ANDROID_ALJNIOBJECT_H
