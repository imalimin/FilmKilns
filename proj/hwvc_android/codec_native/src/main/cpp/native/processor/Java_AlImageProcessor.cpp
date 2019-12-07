/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "HwJavaNativeHelper.h"
#include "AlImageProcessor.h"
#include "HwAndroidWindow.h"

#ifdef __cplusplus
extern "C" {
#endif

static JMethodDescription mOnSave = {"Java_com_lmy_hwvcnative_processor_AlImageProcessor",
                                     "onSave", "(ILjava/lang/String;Ljava/lang/String;)V"};

static AlImageProcessor *getHandler(jlong handler) {
    return reinterpret_cast<AlImageProcessor *>(handler);
}

static void bindListener(jlong handler) {
    getHandler(handler)->setOnSaveListener([handler](int32_t code,
                                                     const char *msg,
                                                     const char *path) {
        jobject jObject = nullptr;
        JNIEnv *pEnv = nullptr;
        jmethodID methodID = nullptr;
        if (HwJavaNativeHelper::getInstance()->findEnv(&pEnv) &&
            HwJavaNativeHelper::getInstance()->findJObject(handler, &jObject) &&
            HwJavaNativeHelper::getInstance()->findMethod(handler,
                                                          mOnSave,
                                                          &methodID)) {
            jstring jMsg = pEnv->NewStringUTF(msg);
            jstring jPath = pEnv->NewStringUTF(path);
            pEnv->CallVoidMethod(jObject, methodID, static_cast<jint>(code), jMsg, jPath);
            pEnv->DeleteLocalRef(jPath);
            pEnv->DeleteLocalRef(jMsg);
        }
    });
}

JNIEXPORT jlong JNICALL Java_com_lmy_hwvcnative_processor_AlImageProcessor_create
        (JNIEnv *env, jobject thiz) {
    AlImageProcessor *p = new AlImageProcessor();
    p->post([] {
        HwJavaNativeHelper::getInstance()->attachThread();
    });
    jlong handler = reinterpret_cast<jlong>(p);
    HwJavaNativeHelper::getInstance()->registerAnObject(env, handler, thiz);
    bindListener(handler);
    return handler;
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AlImageProcessor_updateWindow
        (JNIEnv *env, jobject thiz, jlong handler, jobject surface) {
    if (handler) {
        getHandler(handler)->updateWindow(new HwAndroidWindow(env, surface));
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AlImageProcessor_setCanvas
        (JNIEnv *env, jobject thiz, jlong handler, jint w, jint h, jint color) {
    if (handler) {
        getHandler(handler)->setCanvas(w, h, color);
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AlImageProcessor_release
        (JNIEnv *env, jobject thiz, jlong handler) {
    if (handler) {
        AlImageProcessor *p = getHandler(handler);
        p->post([] {
            HwJavaNativeHelper::getInstance()->detachThread();
        });
        delete p;
    }
    HwJavaNativeHelper::getInstance()->unregisterAnObject(env, handler);
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AlImageProcessor_invalidate
        (JNIEnv *env, jobject thiz, jlong handler) {
    if (handler) {
        getHandler(handler)->invalidate();
    }
}

JNIEXPORT jint JNICALL Java_com_lmy_hwvcnative_processor_AlImageProcessor_addLayer
        (JNIEnv *env, jobject thiz, jlong handler, jstring path) {
    if (handler) {
        const char *pPath = env->GetStringUTFChars(path, JNI_FALSE);
        std::string str(pPath);
        env->ReleaseStringUTFChars(path, pPath);
        return getHandler(handler)->addLayer(str.c_str());
    }
    return Hw::FAILED.code;
}

JNIEXPORT jint JNICALL Java_com_lmy_hwvcnative_processor_AlImageProcessor_moveLayerIndex
        (JNIEnv *env, jobject thiz, jlong handler, jint id, jint index) {
    if (handler) {
        return getHandler(handler)->moveLayerIndex(id, index).code;
    }
    return Hw::FAILED.code;
}

JNIEXPORT jint JNICALL Java_com_lmy_hwvcnative_processor_AlImageProcessor_removeLayer
        (JNIEnv *env, jobject thiz, jlong handler, jint id) {
    if (handler) {
        return getHandler(handler)->removeLayer(id).code;
    }
    return Hw::FAILED.code;
}

JNIEXPORT jint JNICALL Java_com_lmy_hwvcnative_processor_AlImageProcessor_setScale
        (JNIEnv *env, jobject thiz, jlong handler, jint id, jint scaleNum, jint scaleDen) {
    if (handler) {
        return getHandler(handler)->setScale(id, AlRational(scaleNum, scaleDen)).code;
    }
    return Hw::FAILED.code;
}

JNIEXPORT jint JNICALL Java_com_lmy_hwvcnative_processor_AlImageProcessor_postScale
        (JNIEnv *env, jobject thiz, jlong handler, jint id, jint dsNum, jint dsDen) {
    if (handler) {
        return getHandler(handler)->postScale(id, AlRational(dsNum, dsDen)).code;
    }
    return Hw::FAILED.code;
}

JNIEXPORT jint JNICALL Java_com_lmy_hwvcnative_processor_AlImageProcessor_setRotation
        (JNIEnv *env, jobject thiz, jlong handler, jint id, jint rNum, jint rDen) {
    if (handler) {
        return getHandler(handler)->setRotation(id, AlRational(rNum, rDen)).code;
    }
    return Hw::FAILED.code;
}

JNIEXPORT jint JNICALL Java_com_lmy_hwvcnative_processor_AlImageProcessor_postRotation
        (JNIEnv *env, jobject thiz, jlong handler, jint id, jint drNum, jint drDen) {
    if (handler) {
        return getHandler(handler)->postRotation(id, AlRational(drNum, drDen)).code;
    }
    return Hw::FAILED.code;
}

JNIEXPORT jint JNICALL Java_com_lmy_hwvcnative_processor_AlImageProcessor_setTranslate
        (JNIEnv *env, jobject thiz, jlong handler, jint id, jfloat x, jfloat y) {
    if (handler) {
        return getHandler(handler)->setTranslate(id, x, y).code;
    }
    return Hw::FAILED.code;
}

JNIEXPORT jint JNICALL Java_com_lmy_hwvcnative_processor_AlImageProcessor_postTranslate
        (JNIEnv *env, jobject thiz, jlong handler, jint id, jfloat dx, jfloat dy) {
    if (handler) {
        return getHandler(handler)->postTranslate(id, dx, dy).code;
    }
    return Hw::FAILED.code;
}

JNIEXPORT jint JNICALL Java_com_lmy_hwvcnative_processor_AlImageProcessor_setAlpha
        (JNIEnv *env, jobject thiz, jlong handler, jint id, jfloat alpha) {
    if (handler) {
        return getHandler(handler)->setAlpha(id, alpha).code;
    }
    return Hw::FAILED.code;
}

JNIEXPORT jint JNICALL Java_com_lmy_hwvcnative_processor_AlImageProcessor_getLayer
        (JNIEnv *env, jobject thiz, jlong handler, jfloat x, jfloat y) {
    if (handler) {
        return getHandler(handler)->getLayer(x, y);
    }
    return Hw::FAILED.code;
}

JNIEXPORT jint JNICALL Java_com_lmy_hwvcnative_processor_AlImageProcessor_cropLayer
        (JNIEnv *env, jobject thiz, jlong handler, jint id, jfloat left, jfloat top,
         jfloat right, jfloat bottom) {
    if (handler) {
        return getHandler(handler)->cropLayer(id, left, top, right, bottom).code;
    }
    return Hw::FAILED.code;
}

JNIEXPORT jint JNICALL Java_com_lmy_hwvcnative_processor_AlImageProcessor_save
        (JNIEnv *env, jobject thiz, jlong handler, jstring path) {
    HwResult ret = Hw::FAILED.code;
    if (handler) {
        const char *pPath = env->GetStringUTFChars(path, JNI_FALSE);
        ret = getHandler(handler)->save(std::string(pPath));
        env->ReleaseStringUTFChars(path, pPath);
    }
    return ret.code;
}

#ifdef __cplusplus
}
#endif

