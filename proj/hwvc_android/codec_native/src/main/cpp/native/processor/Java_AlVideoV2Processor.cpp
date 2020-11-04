/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "platform/android/AlJavaRuntime.h"
#include "platform/android/AlJNIObject.h"
#include "AlVideoV2Processor.h"
#include "HwAndroidWindow.h"

#ifdef __cplusplus
extern "C" {
#endif

static AlJNIObject::Method midOnNativeMessage = {
        "com/lmy/hwvcnative/processor/AlVideoV2Processor",
        "onDispatchNativeMessage", "(IIJJ)V"};

static AlJNIObject::Method midTrackUpdate = {
        "com/lmy/hwvcnative/processor/AlVideoV2Processor",
        "onNativeTrackUpdate", "([B)V"};

static AlJNIObject::Method midMonitorUpdate = {
        "com/lmy/hwvcnative/processor/AlVideoV2Processor",
        "onNativeMonitorUpdate", "([B[B)V"};

static AlVideoV2Processor *getHandler(jlong handler) {
    return reinterpret_cast<AlVideoV2Processor *>(handler);
}

static void bindListener(AlVideoV2Processor *p) {
    p->setPlayProgressListener([p](int64_t timeInUS, int64_t duration) {
        AlJNIObject *obj = nullptr;
        if (AlJavaRuntime::getInstance().findObj(p, &obj)) {
            al_jni_call_void(obj, midOnNativeMessage, 0, 0, timeInUS, duration);
        }
    });
    p->setOnTrackUpdateListener([p](std::shared_ptr<AlMediaTrack> track) {
        AlJNIObject *obj = nullptr;
        JNIEnv *env = nullptr;
        if (AlJavaRuntime::getInstance().findObj(p, &obj) && AlJavaRuntime::getInstance().findEnv(&env)) {
            auto buf = track->data();
            if (nullptr == buf || buf->size() <= 0) {
                return;
            }
            buf->rewind();
            AlLogI("Java_AlVideoV2Processor", "parcel %d", buf->size());
            if (JNI_TRUE == env->ExceptionCheck()) {
                env->ExceptionDescribe();
                env->ExceptionClear();
            }
            auto data = env->NewByteArray(buf->size());
            env->SetByteArrayRegion(data, 0, buf->size(),
                                    reinterpret_cast<const jbyte *>(buf->data()));
            al_jni_call_void(obj, midTrackUpdate, data);
            env->DeleteLocalRef(data);
        }
    });
    p->setOnMonitorListener([p](int monitorState, std::shared_ptr<AlVector<std::shared_ptr<FkUnitDesc>>> unitsDesc,
                               std::shared_ptr<FkMsgState> state) {
        AlJNIObject *obj = nullptr;
        JNIEnv *env = nullptr;
        if (AlJavaRuntime::getInstance().findObj(p, &obj) && AlJavaRuntime::getInstance().findEnv(&env)) {
            AlLogI("alimin", "%" PRId64 ", %" PRId64 ", %p", state->what, state->costTimeInUS, (void *)obj);
//            auto buf0 = state->data();
//            auto buf1 = state->data();
//            if (nullptr == buf0 || buf0->size() <= 0
//                || nullptr == buf1 || buf1->size() <= 0) {
//                return;
//            }
//            buf0->rewind();
//            buf1->rewind();
//            if (JNI_TRUE == env->ExceptionCheck()) {
//                env->ExceptionDescribe();
//                env->ExceptionClear();
//            }
//            auto data0 = env->NewByteArray(buf0->size());
//            env->SetByteArrayRegion(data0, 0, buf0->size(),
//                                    reinterpret_cast<const jbyte *>(buf0->data()));
//            auto data1 = env->NewByteArray(buf1->size());
//            env->SetByteArrayRegion(data1, 0, buf1->size(),
//                                    reinterpret_cast<const jbyte *>(buf1->data()));
//            al_jni_call_void(obj, midMonitorUpdate, data0, data1);
//            env->DeleteLocalRef(data1);
//            env->DeleteLocalRef(data0);
        }
    });
}

JNIEXPORT jlong JNICALL Java_com_lmy_hwvcnative_processor_AlVideoV2Processor_create
        (JNIEnv *env, jobject thiz) {
    auto *p = new AlVideoV2Processor();
    auto obj = env->NewGlobalRef(thiz);
    p->post([env, p, obj] {
        AlJavaRuntime::getInstance().attachThread();
        AlJavaRuntime::getInstance().attach(p, obj, false);
    });
    p->setOnDestroyListener(AlRunnable::runEmptyArgs([p]() {
        AlLogI("Java_AlVideoV2Processor", "release");
        AlJavaRuntime::getInstance().detach(p);
        AlJavaRuntime::getInstance().detachThread();
    }));
    bindListener(p);
    return reinterpret_cast<jlong>(p);
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AlVideoV2Processor_release
        (JNIEnv *env, jobject thiz, jlong handler) {
    if (handler) {
        AlVideoV2Processor *p = getHandler(handler);
        p->release();
        delete p;
    }
}

JNIEXPORT jint JNICALL Java_com_lmy_hwvcnative_processor_AlVideoV2Processor_addTrack
        (JNIEnv *env, jobject thiz, jlong handler, jint type, jstring path,
         jlong seqIn, jlong seqOut, jlong trimIn, jlong trimOut) {
    jint track = -1;
    if (handler) {
        const char *ptr = env->GetStringUTFChars(path, JNI_FALSE);
        track = getHandler(handler)->addTrack((AlMediaTrack::kType) type, std::string(ptr),
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

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AlVideoV2Processor_updateWindow
        (JNIEnv *env, jobject thiz, jlong handler, jobject surface) {
    if (handler) {
        getHandler(handler)->updateWindow(new HwAndroidWindow(env, surface));
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AlVideoV2Processor_setCanvasBackground
        (JNIEnv *env, jobject thiz, jlong handler, jint type) {
    if (handler) {
        getHandler(handler)->setCanvasBackground(type);
    }
}

JNIEXPORT jint JNICALL Java_com_lmy_hwvcnative_processor_AlVideoV2Processor_getLayer
        (JNIEnv *env, jobject thiz, jlong handler, jfloat x, jfloat y) {
    if (handler) {
        return getHandler(handler)->getLayer(x, y);
    }
    return Hw::FAILED.code;
}

JNIEXPORT jint JNICALL Java_com_lmy_hwvcnative_processor_AlVideoV2Processor_postTranslate
        (JNIEnv *env, jobject thiz, jlong handler, jint id, jfloat x, jfloat y) {
    if (handler) {
        return getHandler(handler)->postTranslate(id, x, y).code;
    }
    return Hw::FAILED.code;
}

JNIEXPORT jint JNICALL Java_com_lmy_hwvcnative_processor_AlVideoV2Processor_postScale
        (JNIEnv *env, jobject thiz, jlong handler, jint id, jint dsNum, jint dsDen,
         jfloat anchorX, jfloat anchorY) {
    if (handler) {
        return getHandler(handler)->postScale(id, AlRational(dsNum, dsDen),
                                              AlPointF(anchorX, anchorY)).code;
    }
    return Hw::FAILED.code;
}

#ifdef __cplusplus
}
#endif