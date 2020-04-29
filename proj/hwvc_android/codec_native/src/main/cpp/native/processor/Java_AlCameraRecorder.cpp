/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/
#include "platform/android/AlJavaNativeHelper.h"
#include "HwCameraRecorder.h"
#include "../include/HwAndroidWindow.h"

#ifdef __cplusplus
extern "C" {
#endif

static JMethodDescription cOnHandleMessage = {
        "com/lmy/hwvcnative/processor/AlCameraRecorder",
        "onHandleMessage", "(II)V"};
static JMethodDescription vRecordProgressDesc = {
        "com/lmy/hwvcnative/processor/AlCameraRecorder",
        "onRecordProgress", "(J)V"};
static JMethodDescription midOnNativePrepared = {
        "com/lmy/hwvcnative/processor/AlCameraRecorder",
        "onNativePrepared", "(I)V"};

static HwCameraRecorder *getHandler(jlong handler) {
    return reinterpret_cast<HwCameraRecorder *>(handler);
}

static void bindListener(jlong handler) {
    getHandler(handler)->setRecordListener([handler](int64_t timeInUs) {
        jobject jObject = nullptr;
        JNIEnv *pEnv = nullptr;
        jmethodID mid = nullptr;
        if (AlJavaNativeHelper::getInstance()->findEnv(&pEnv) &&
            AlJavaNativeHelper::getInstance()->findJObject(handler, &jObject) &&
            AlJavaNativeHelper::getInstance()->findMethod(handler,
                                                          vRecordProgressDesc,
                                                          &mid)) {
            pEnv->CallVoidMethod(jObject, mid, static_cast<jlong>(timeInUs));
        }
    });
    getHandler(handler)->setOnNativeReadyListener([handler](int32_t oesTex) {
        jobject jObject = nullptr;
        JNIEnv *pEnv = nullptr;
        jmethodID mid = nullptr;
        if (AlJavaNativeHelper::getInstance()->findEnv(&pEnv) &&
            AlJavaNativeHelper::getInstance()->findJObject(handler, &jObject) &&
            AlJavaNativeHelper::getInstance()->findMethod(handler,
                                                          midOnNativePrepared,
                                                          &mid)) {
            pEnv->CallVoidMethod(jObject, mid, static_cast<jint>(oesTex));
        }
    });

}

JNIEXPORT jlong JNICALL Java_com_lmy_hwvcnative_processor_AlCameraRecorder_create
        (JNIEnv *env, jobject thiz) {
    HwCameraRecorder *p = new HwCameraRecorder();
    p->post([] {
        AlJavaNativeHelper::getInstance()->attachThread();
    });
    jlong handler = reinterpret_cast<jlong>(p);
    AlJavaNativeHelper::getInstance()->registerAnObject(env, handler, thiz);
    bindListener(handler);
    return handler;
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AlCameraRecorder_postEvent
        (JNIEnv *env, jobject thiz, jlong handler, jint what) {
    if (handler) {
        int w = what;
        getHandler(handler)->runOnCameraContext([handler, w]() {
            jobject jObject = nullptr;
            JNIEnv *pEnv = nullptr;
            jmethodID methodID = nullptr;
            if (AlJavaNativeHelper::getInstance()->findEnv(&pEnv) &&
                AlJavaNativeHelper::getInstance()->findJObject(handler, &jObject) &&
                AlJavaNativeHelper::getInstance()->findMethod(handler,
                                                              cOnHandleMessage,
                                                              &methodID)) {
                switch (w) {
                    case 4: {
                        pEnv->CallVoidMethod(jObject, methodID, w, 0);
                        break;
                    }
                    case 2: {
                        pEnv->CallVoidMethod(jObject, methodID, w, 0);
                        break;
                    }
                    default:
                        pEnv->CallVoidMethod(jObject, methodID, w, 0);
                }
                pEnv->ExceptionCheck();
                pEnv->ExceptionClear();
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
        HwCameraRecorder *p = getHandler(handler);
        p->post([] {
            AlLogI("Java_AlCameraRecorder", "release");
            AlJavaNativeHelper::getInstance()->detachThread();
        });
        p->release();
        delete p;
    }
    AlJavaNativeHelper::getInstance()->unregisterAnObject(env, handler);
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