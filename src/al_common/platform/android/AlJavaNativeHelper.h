/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALJAVANATIVEHELPER_H
#define HWVC_ANDROID_ALJAVANATIVEHELPER_H

#include "Object.h"
#include <jni.h>
#include <log.h>
#include <map>
#include <string>
#include "SimpleLock.h"

using namespace std;
typedef struct {
    string cls;
    string name;
    string sign;
} JMethodDescription;

/**
 * A simple helper for jni.
 * 1. helper.attach on JNI_OnLoad. And helper.detach on JNI_OnUnload.
 * 2. helper.registerAnObject with C object and Java object.
 * 3. helper.attachThread on other child thread where you want.
 * 4. helper.detachThread on child thread when you exit.
 * 5. Finally, helper.unregisterAnObject.
 */
class AlJavaNativeHelper : public Object {
public:
    static string getClassName(JNIEnv *env, jobject object);

    static int getAndroidApi();

public:
    static AlJavaNativeHelper *getInstance();

    virtual ~AlJavaNativeHelper();

    void attach(JavaVM *vm);

    void detach();

    void registerAnObject(JNIEnv *env, jlong handler, jobject jHandler);

    void unregisterAnObject(JNIEnv *env, jlong handler);

    bool attachThread();

    void detachThread();

    bool findEnv(JNIEnv **env);

    bool findJObject(jlong handler, jobject *jObject);

    bool findMethod(jlong handler, JMethodDescription method, jmethodID *methodID);

    bool callMethod(jlong handler, JMethodDescription method, ...);

private:
    AlJavaNativeHelper();

    AlJavaNativeHelper(const AlJavaNativeHelper &object);

    AlJavaNativeHelper &operator=(const AlJavaNativeHelper &object);

private:
    static AlJavaNativeHelper *instance;
    JavaVM *jvm = nullptr;
    map<jlong, jobject> objMap;
    map<long, JNIEnv *> envMap;
    map<string, jmethodID> methodMap;
};


#endif //HWVC_ANDROID_ALJAVANATIVEHELPER_H
