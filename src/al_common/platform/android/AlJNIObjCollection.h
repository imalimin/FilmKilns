/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALJNIOBJCOLLECTION_H
#define HWVC_ANDROID_ALJNIOBJCOLLECTION_H

#include "Object.h"
#include "AlJNIObject.h"
#include "jni.h"
#include <map>

al_def_class(AlJNIObjCollection) {
public:
    AlJNIObjCollection();

    ~AlJNIObjCollection();

    bool attach(JNIEnv *env, Object *o, jobject j, bool reqGlobalRef = true);

    void detach(JNIEnv *env, Object *o);

    bool findObj(Object *o, AlJNIObject **obj);

    size_t size();

private:
    std::map<Object *, AlJNIObject *> map;
};


#endif //HWVC_ANDROID_ALJNIOBJCOLLECTION_H
