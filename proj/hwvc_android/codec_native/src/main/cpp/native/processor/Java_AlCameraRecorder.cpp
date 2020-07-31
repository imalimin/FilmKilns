/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/
#include "platform/android/AlJavaRuntime.h"
#include "platform/android/AlJNIObject.h"
#include "AlCameraRecorder.h"
#include "HwAndroidWindow.h"

#ifdef __cplusplus
extern "C" {
#endif

static AlJNIObject::Method cOnHandleMessage = {
        "com/lmy/hwvcnative/processor/AlCameraRecorder",
        "onHandleMessage", "(II)V"};
static AlJNIObject::Method vRecordProgressDesc = {
        "com/lmy/hwvcnative/processor/AlCameraRecorder",
        "onRecordProgress", "(J)V"};
static AlJNIObject::Method midOnNativePrepared = {
        "com/lmy/hwvcnative/processor/AlCameraRecorder",
        "onNativePrepared", "(I)V"};

static AlCameraRecorder *getHandler(jlong handler) {
    return reinterpret_cast<AlCameraRecorder *>(handler);
}

static void bindListener(AlCameraRecorder *p) {
    p->setRecordListener([p](int64_t timeInUs) {
        AlJNIObject *obj = nullptr;
        if (AlJavaRuntime::getInstance().findObj(p, &obj)) {
            al_jni_call_void(obj, vRecordProgressDesc, timeInUs);
        }
    });
    p->setOnNativeReadyListener([p](int32_t oesTex) {
        AlJNIObject *obj = nullptr;
        if (AlJavaRuntime::getInstance().findObj(p, &obj)) {
            al_jni_call_void(obj, midOnNativePrepared, oesTex);
        }
    });

}

JNIEXPORT jlong JNICALL Java_com_lmy_hwvcnative_processor_AlCameraRecorder_create
        (JNIEnv *env, jobject thiz) {
    AlCameraRecorder *p = new AlCameraRecorder();
    auto obj = env->NewGlobalRef(thiz);
    p->post([env, p, obj] {
        AlJavaRuntime::getInstance().attachThread();
        AlJavaRuntime::getInstance().attach(p, obj, false);
    });
    bindListener(p);
    return reinterpret_cast<jlong>(p);
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AlCameraRecorder_postEvent
        (JNIEnv *env, jobject thiz, jlong handler, jint what) {
    if (handler) {
        auto *p = getHandler(handler);
        p->runOnCameraContext([p, what]() {
            AlJNIObject *obj = nullptr;
            if (AlJavaRuntime::getInstance().findObj(p, &obj)) {
                al_jni_call_void(obj, cOnHandleMessage, what, 0);
            }
        });
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AlCameraRecorder_setOutputFilePath
        (JNIEnv *env, jobject thiz, jlong handler, jstring filePath) {
    if (handler) {
        const char *pPilePath = env->GetStringUTFChars(filePath, JNI_FALSE);
        std::string pathStr(pPilePath);
        env->ReleaseStringUTFChars(filePath, pPilePath);
        getHandler(handler)->setOutputFilePath(pathStr);
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AlCameraRecorder_setFormat
        (JNIEnv *env, jobject thiz, jlong handler, jint width, jint height, jint sampleFormat,
         jint channels, jint sampleRate) {
    if (handler) {
        getHandler(handler)->setFormat(width, height, HwSampleFormat(
                static_cast<HwFrameFormat>(sampleFormat), channels, sampleRate));
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AlCameraRecorder_updateWindow
        (JNIEnv *env, jobject thiz, jlong handler, jobject surface) {
    if (handler) {
        getHandler(handler)->updateWindow(new HwAndroidWindow(env, surface));
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AlCameraRecorder_start
        (JNIEnv *env, jobject thiz, jlong handler) {
    if (handler) {
        getHandler(handler)->start();
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AlCameraRecorder_pause
        (JNIEnv *env, jobject thiz, jlong handler) {
    if (handler) {
        getHandler(handler)->pause();
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AlCameraRecorder_release
        (JNIEnv *env, jobject thiz, jlong handler) {
    if (handler) {
        AlCameraRecorder *p = getHandler(handler);
        p->release(AlRunnable::runEmptyArgs([p]() {
            AlLogI("Java_AlCameraRecorder", "release");
            AlJavaRuntime::getInstance().detach(p);
            AlJavaRuntime::getInstance().detachThread();
        }));
        delete p;
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AlCameraRecorder_invalidate
        (JNIEnv *env, jobject thiz, jlong handler, jfloatArray matrix, jlong tsInNs, jint cw,
         jint ch) {
    if (handler) {
        if (matrix) {
            jfloat *pMatrix = env->GetFloatArrayElements(matrix, JNI_FALSE);
            AlMatrix m = AlMatrix::fromArray(pMatrix);
            env->ReleaseFloatArrayElements(matrix, pMatrix, 0);
            getHandler(handler)->setCameraSize(cw, ch);
            getHandler(handler)->invalidate(&m, tsInNs);
        }
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AlCameraRecorder_setFilter
        (JNIEnv *env, jobject thiz, jlong handler, jlong filter) {
    if (handler && filter) {
        getHandler(handler)->setFilter(reinterpret_cast<HwAbsFilter *>(filter));
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AlCameraRecorder_backward
        (JNIEnv *env, jobject thiz, jlong handler) {
    if (handler) {
        getHandler(handler)->backward();
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AlCameraRecorder_setVideoBitLevel
        (JNIEnv *env, jobject thiz, jlong handler, jint level) {
    if (handler) {
        getHandler(handler)->setVideoBitLevel(level);
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AlCameraRecorder_setProfile
        (JNIEnv *env, jobject thiz, jlong handler, jstring profile) {
    if (handler) {
        const char *str = env->GetStringUTFChars(profile, JNI_FALSE);
        getHandler(handler)->setProfile(std::string(str));
        env->ReleaseStringUTFChars(profile, str);
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AlCameraRecorder_setPreset
        (JNIEnv *env, jobject thiz, jlong handler, jstring profile) {
    if (handler) {
        const char *str = env->GetStringUTFChars(profile, JNI_FALSE);
        getHandler(handler)->setPreset(std::string(str));
        env->ReleaseStringUTFChars(profile, str);
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AlCameraRecorder_setEnableHardware
        (JNIEnv *env, jobject thiz, jlong handler, jboolean enable) {
    if (handler) {
        getHandler(handler)->setEnableHardware(JNI_TRUE == enable);
    }
}

#ifdef __cplusplus
}
#endif