/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "platform/android/AlJNIEnv.h"
#include "platform/android/AlJNIObject.h"
#include "AlVideoV2Processor.h"

#ifdef __cplusplus
extern "C" {
#endif

static AlVideoV2Processor *getHandler(jlong handler) {
    return reinterpret_cast<AlVideoV2Processor *>(handler);
}

JNIEXPORT jlong JNICALL Java_com_lmy_hwvcnative_processor_AlVideoV2Processor_create
        (JNIEnv *env, jobject thiz) {
    auto *p = new AlVideoV2Processor();
    auto obj = env->NewGlobalRef(thiz);
    p->post([env, p, obj] {
        AlJNIEnv::getInstance().attachThread();
        AlJNIEnv::getInstance().attach(p, obj, false);
    });
    return reinterpret_cast<jlong>(p);
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AlVideoV2Processor_release
        (JNIEnv *env, jobject thiz, jlong handler) {
    if (handler) {
        AlVideoV2Processor *p = getHandler(handler);
        p->release(AlRunnable::runEmptyArgs([p]() {
            AlLogI("Java_AlVideoV2Processor", "release");
            AlJNIEnv::getInstance().detach(p);
            AlJNIEnv::getInstance().detachThread();
        }));
        delete p;
    }
}

#ifdef __cplusplus
}
#endif