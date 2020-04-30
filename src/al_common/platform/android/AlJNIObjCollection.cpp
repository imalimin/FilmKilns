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
    AlLogI(TAG, "left %d.", map.size());
}

bool AlJNIObjCollection::attach(JNIEnv *env, Object *o, jobject j, bool reqGlobalRef) {
    auto itr = map.find(o);
    if (map.end() != itr) {
        AlLogI(TAG, "failed. Attach repeat.");
        return false;
    }
    jobject obj = reqGlobalRef ? env->NewGlobalRef(j) : j;
    map.insert({o, new AlJNIObject(env, obj)});
    return true;
}

void AlJNIObjCollection::detach(JNIEnv *env, Object *o) {
    auto itr = map.find(o);
    if (map.end() == itr) {
        AlLogI(TAG, "failed. Attach first pls.");
    }
    auto tmp = itr->second;
    env->DeleteGlobalRef(tmp->o);
    delete tmp;
    map.erase(itr);
}

bool AlJNIObjCollection::findObj(Object *o, AlJNIObject **obj) {
    auto itr = map.find(o);
    if (map.end() == itr) {
        AlLogI(TAG, "failed. Attach first pls.");
        return false;
    }
    *obj = itr->second;
    return true;
}

size_t AlJNIObjCollection::size() {
    return map.size();
}
