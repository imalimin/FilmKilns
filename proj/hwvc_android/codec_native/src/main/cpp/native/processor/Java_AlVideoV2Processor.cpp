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

static AlJNIObject::Method midOnNativeMessage = {
        "com/lmy/hwvcnative/processor/AlVideoV2Processor",
        "onDispatchNativeMessage", "(IIJJ)V"};

static AlVideoV2Processor *getHandler(jlong handler) {
    return reinterpret_cast<AlVideoV2Processor *>(handler);
}

static void bindListener(AlVideoV2Processor *p) {
    p->setPlayProgressListener([p](int64_t timeInUS, int64_t duration) {
        AlJNIObject *obj = nullptr;
        if (AlJNIEnv::getInstance().findObj(p, &obj)) {
            al_jni_call_void(obj, midOnNativeMessage, 0, 0, timeInUS, duration);
        }
    });
}

JNIEXPORT jlong JNICALL Java_com_lmy_hwvcnative_processor_AlVideoV2Processor_create
        (JNIEnv *env, jobject thiz) {
    auto *p = new AlVideoV2Processor();
    auto obj = env->NewGlobalRef(thiz);
    p->post([env, p, obj] {
        AlJNIEnv::getInstance().attachThread();
        AlJNIEnv::getInstance().attach(p, obj, false);
    });
    bindListener(p);
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

JNIEXPORT jint JNICALL Java_com_lmy_hwvcnative_processor_AlVideoV2Processor_addTrack
        (JNIEnv *env, jobject thiz, jlong handler, jint type, jstring path,
         jlong seqIn, jlong seqOut, jlong trimIn, jlong trimOut) {
    jint track = -1;
    if (handler) {
        const char *ptr = env->GetStringUTFChars(path, JNI_FALSE);
        track = getHandler(handler)->addTrack((AlCodec::kMediaType) type, std::string(ptr),
                                              seqIn, seqOut, trimIn, trimOut);
        env->ReleaseStringUTFChars(path, ptr);
    }
    return track;
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AlVideoV2Processor_removeTrack
        (JNIEnv *env, jobject thiz, jlong handler, jint trackID) {
    if (handler) {
        getHandler(handler)->removeTrack(trackID);
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AlVideoV2Processor_start
        (JNIEnv *env, jobject thiz, jlong handler) {
    if (handler) {
        getHandler(handler)->start();
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AlVideoV2Processor_pause
        (JNIEnv *env, jobject thiz, jlong handler) {
    if (handler) {
        getHandler(handler)->pause();
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AlVideoV2Processor_seek
        (JNIEnv *env, jobject thiz, jlong handler, jlong timeInUS) {
    if (handler) {
        getHandler(handler)->seek(timeInUS);
    }
}

#ifdef __cplusplus
}
#endif