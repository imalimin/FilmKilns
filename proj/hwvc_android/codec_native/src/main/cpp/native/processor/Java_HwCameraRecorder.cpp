/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/
#include "../include/HwJavaNativeHelper.h"
#include "HwCameraRecorder.h"
#include "../include/HwAndroidWindow.h"

#ifdef __cplusplus
extern "C" {
#endif

static JMethodDescription cOnHandleMessage = {"Java_com_lmy_hwvcnative_processor_HwCameraRecorder",
                                              "onHandleMessage", "(II)V"};
static JMethodDescription vRecordProgressDesc = {
        "Java_com_lmy_hwvcnative_processor_HwCameraRecorder",
        "onRecordProgress", "(J)V"};
static JMethodDescription midOnNativePrepared = {
        "Java_com_lmy_hwvcnative_processor_HwCameraRecorder",
        "onNativePrepared", "(I)V"};

static HwCameraRecorder *getHandler(jlong handler) {
    return reinterpret_cast<HwCameraRecorder *>(handler);
}

static void bindListener(jlong handler) {
    getHandler(handler)->setRecordListener([handler](int64_t timeInUs) {
        jobject jObject = nullptr;
        JNIEnv *pEnv = nullptr;
        jmethodID mid = nullptr;
        if (HwJavaNativeHelper::getInstance()->findEnv(&pEnv) &&
            HwJavaNativeHelper::getInstance()->findJObject(handler, &jObject) &&
            HwJavaNativeHelper::getInstance()->findMethod(handler,
                                                          vRecordProgressDesc,
                                                          &mid)) {
            pEnv->CallVoidMethod(jObject, mid, static_cast<jlong>(timeInUs));
        }
    });
    getHandler(handler)->setOnNativeReadyListener([handler](int32_t oesTex) {
        jobject jObject = nullptr;
        JNIEnv *pEnv = nullptr;
        jmethodID mid = nullptr;
        if (HwJavaNativeHelper::getInstance()->findEnv(&pEnv) &&
            HwJavaNativeHelper::getInstance()->findJObject(handler, &jObject) &&
            HwJavaNativeHelper::getInstance()->findMethod(handler,
                                                          midOnNativePrepared,
                                                          &mid)) {
            pEnv->CallVoidMethod(jObject, mid, static_cast<jint>(oesTex));
        }
    });

}

JNIEXPORT jlong JNICALL Java_com_lmy_hwvcnative_processor_HwCameraRecorder_create
        (JNIEnv *env, jobject thiz) {
    HwCameraRecorder *p = new HwCameraRecorder();
    p->post([] {
        HwJavaNativeHelper::getInstance()->attachThread();
    });
    jlong handler = reinterpret_cast<jlong>(p);
    HwJavaNativeHelper::getInstance()->registerAnObject(env, handler, thiz);
    bindListener(handler);
    return handler;
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_HwCameraRecorder_postEvent
        (JNIEnv *env, jobject thiz, jlong handler, jint what) {
    if (handler) {
        int w = what;
        getHandler(handler)->runOnCameraContext([handler, w]() {
            jobject jObject = nullptr;
            JNIEnv *pEnv = nullptr;
            jmethodID methodID = nullptr;
            if (HwJavaNativeHelper::getInstance()->findEnv(&pEnv) &&
                HwJavaNativeHelper::getInstance()->findJObject(handler, &jObject) &&
                HwJavaNativeHelper::getInstance()->findMethod(handler,
                                                              cOnHandleMessage,
                                                              &methodID)) {
                AlLogI("Java_HwCameraRecorder", "what %d <<", w);
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
            }
            AlLogI("Java_HwCameraRecorder", "what %d >>", w);
        });
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_HwCameraRecorder_setOutputFilePath
        (JNIEnv *env, jobject thiz, jlong handler, jstring filePath) {
    if (handler) {
        const char *pPilePath = env->GetStringUTFChars(filePath, JNI_FALSE);
        std::string pathStr(pPilePath);
        env->ReleaseStringUTFChars(filePath, pPilePath);
        getHandler(handler)->setOutputFilePath(pathStr);
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_HwCameraRecorder_setFormat
        (JNIEnv *env, jobject thiz, jlong handler, jint width, jint height, jint sampleFormat,
         jint channels, jint sampleRate) {
    if (handler) {
        getHandler(handler)->setFormat(width, height, HwSampleFormat(
                static_cast<HwFrameFormat>(sampleFormat), channels, sampleRate));
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_HwCameraRecorder_updateWindow
        (JNIEnv *env, jobject thiz, jlong handler, jobject surface) {
    if (handler) {
        getHandler(handler)->updateWindow(new HwAndroidWindow(env, surface));
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_HwCameraRecorder_start
        (JNIEnv *env, jobject thiz, jlong handler) {
    if (handler) {
        getHandler(handler)->start();
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_HwCameraRecorder_pause
        (JNIEnv *env, jobject thiz, jlong handler) {
    if (handler) {
        getHandler(handler)->pause();
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_HwCameraRecorder_release
        (JNIEnv *env, jobject thiz, jlong handler) {
    if (handler) {
        HwCameraRecorder *p = getHandler(handler);
        p->post([] {
            AlLogI("Java_HwCameraRecorder", "release");
            HwJavaNativeHelper::getInstance()->detachThread();
        });
        delete p;
    }
    HwJavaNativeHelper::getInstance()->unregisterAnObject(env, handler);
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_HwCameraRecorder_invalidate
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

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_HwCameraRecorder_setFilter
        (JNIEnv *env, jobject thiz, jlong handler, jlong filter) {
    if (handler && filter) {
        getHandler(handler)->setFilter(reinterpret_cast<HwAbsFilter *>(filter));
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_HwCameraRecorder_backward
        (JNIEnv *env, jobject thiz, jlong handler) {
    if (handler) {
        getHandler(handler)->backward();
    }
}

#ifdef __cplusplus
}
#endif