/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlJNIObjCollection.h"
#include "AlLogcat.h"

#define TAG "AlJNIObjCollection"

AlJNIObjCollection::AlJNIObjCollection() : Object() {

}

AlJNIObjCollection::~AlJNIObjCollection() {
    AlLogI(TAG, "left %d.", size());
}

bool AlJNIObjCollection::attach(JNIEnv *env, Object *o, jobject j, bool reqGlobalRef) {
    AlLogI(TAG, "start %p", this);
    for (auto &it : mObjMap) {
        AlLogI(TAG, "%p, %p", it.first, it.second);
    }
    auto itr = mObjMap.find(o);
    if (mObjMap.end() != itr) {
        AlLogI(TAG, "failed. Attach repeat.");
        return false;
    }
    jobject obj = reqGlobalRef ? env->NewGlobalRef(j) : j;
    mObjMap.insert({o, new AlJNIObject(env, obj)});
    AlLogI(TAG, "end %p", this);
    for (auto &it : mObjMap) {
        AlLogI(TAG, "%p, %p", it.first, it.second);
    }
    return true;
}

void AlJNIObjCollection::detach(JNIEnv *env, Object *o) {
    auto itr = mObjMap.find(o);
    if (mObjMap.end() == itr) {
        AlLogI(TAG, "failed. Attach first pls.");
        return;
    }
    auto tmp = itr->second;
    env->DeleteGlobalRef(tmp->o);
    delete tmp;
    mObjMap.erase(itr);
}

bool AlJNIObjCollection::findObj(Object *o, AlJNIObject **obj) {
    auto itr = mObjMap.find(o);
    if (mObjMap.end() == itr) {
        AlLogI(TAG, "failed. Attach first pls.");
        return false;
    }
    *obj = itr->second;
    return true;
}

size_t AlJNIObjCollection::size() {
    return mObjMap.size();
}
