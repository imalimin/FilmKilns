/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "platform/android/AlJavaNativeHelper.h"
#include "AlImageProcessor.h"
#include "HwAndroidWindow.h"

#ifdef __cplusplus
extern "C" {
#endif

static JMethodDescription mOnSave = {"Java_com_lmy_hwvcnative_processor_AlImageProcessor",
                                     "onSave", "(ILjava/lang/String;Ljava/lang/String;)V"};
static JMethodDescription mOnLayerInfo = {"Java_com_lmy_hwvcnative_processor_AlImageProcessor",
                                          "onLayerInfo", "([I[I[I)V"};

static AlImageProcessor *getHandler(jlong handler) {
    return reinterpret_cast<AlImageProcessor *>(handler);
}

static void bindListener(jlong handler) {
    getHandler(handler)->setOnSaveListener([handler](int32_t code,
                                                     const char *msg,
                                                     const char *path) {
        jobject jObject = nullptr;
        JNIEnv *pEnv = nullptr;
        jmethodID mid = nullptr;
        if (AlJavaNativeHelper::getInstance()->findEnv(&pEnv) &&
            AlJavaNativeHelper::getInstance()->findJObject(handler, &jObject) &&
            AlJavaNativeHelper::getInstance()->findMethod(handler,
                                                          mOnSave,
                                                          &mid)) {
            jstring jMsg = pEnv->NewStringUTF(msg);
            jstring jPath = pEnv->NewStringUTF(path);
            pEnv->CallVoidMethod(jObject, mid, static_cast<jint>(code), jMsg, jPath);
            pEnv->DeleteLocalRef(jPath);
            pEnv->DeleteLocalRef(jMsg);
        }
    });
    getHandler(handler)->setOnLayerInfoListener([handler](std::vector<int32_t> ids,
                                                          std::vector<int32_t> ws,
                                                          std::vector<int32_t> hs) {
        jobject jObject = nullptr;
        JNIEnv *pEnv = nullptr;
        jmethodID mid = nullptr;
        if (AlJavaNativeHelper::getInstance()->findEnv(&pEnv) &&
            AlJavaNativeHelper::getInstance()->findJObject(handler, &jObject) &&
            AlJavaNativeHelper::getInstance()->findMethod(handler,
                                                          mOnLayerInfo,
                                                          &mid)) {
            jintArray jIds = pEnv->NewIntArray(ids.size());
            jintArray jWs = pEnv->NewIntArray(ws.size());
            jintArray jHs = pEnv->NewIntArray(hs.size());
            pEnv->SetIntArrayRegion(jIds, 0, ids.size(), ids.data());
            pEnv->SetIntArrayRegion(jWs, 0, ws.size(), ws.data());
            pEnv->SetIntArrayRegion(jHs, 0, hs.size(), hs.data());
            pEnv->CallVoidMethod(jObject, mid, jIds, jWs, jHs);
            pEnv->DeleteLocalRef(jHs);
            pEnv->DeleteLocalRef(jWs);
            pEnv->DeleteLocalRef(jIds);
        }

    });
}

JNIEXPORT jlong JNICALL Java_com_lmy_hwvcnative_processor_AlImageProcessor_create
        (JNIEnv *env, jobject thiz) {
    AlImageProcessor *p = new AlImageProcessor();
    p->post([] {
        AlJavaNativeHelper::getInstance()->attachThread();
    });
    jlong handler = reinterpret_cast<jlong>(p);
    AlJavaNativeHelper::getInstance()->registerAnObject(env, handler, thiz);
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
        (JNIEnv *env, jobject thiz, jlong handler, jint w, jint h, jint loc, jint color) {
    if (handler) {
        getHandler(handler)->setCanvas(w, h, AlRectLoc::value(loc), AlColor(color));
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AlImageProcessor_release
        (JNIEnv *env, jobject thiz, jlong handler) {
    if (handler) {
        AlImageProcessor *p = getHandler(handler);
        p->post([] {
            AlJavaNativeHelper::getInstance()->detachThread();
        });
        p->release();
        delete p;
    }
    AlJavaNativeHelper::getInstance()->unregisterAnObject(env, handler);
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
        (JNIEnv *env, jobject thiz, jlong handler, jint id, jint dsNum, jint dsDen,
         jfloat anchorX, jfloat anchorY) {
    if (handler) {
        return getHandler(handler)->postScale(id, AlRational(dsNum, dsDen),
                                              AlPointF(anchorX, anchorY)).code;
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
        (JNIEnv *env, jobject thiz, jlong handler, jint id, jint drNum, jint drDen,
         jfloat anchorX, jfloat anchorY) {
    if (handler) {
        return getHandler(handler)->postRotation(id, AlRational(drNum, drDen),
                                                 AlPointF(anchorX, anchorY)).code;
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

JNIEXPORT jint JNICALL Java_com_lmy_hwvcnative_processor_AlImageProcessor_ensureCropLayer
        (JNIEnv *env, jobject thiz, jlong handler, jint id, jfloat left, jfloat top,
         jfloat right, jfloat bottom) {
    if (handler) {
        return getHandler(handler)->cropLayer(id, left, top, right, bottom).code;
    }
    return Hw::FAILED.code;
}

JNIEXPORT jint JNICALL Java_com_lmy_hwvcnative_processor_AlImageProcessor_cropCanvas
        (JNIEnv *env, jobject thiz, jlong handler, jfloat left, jfloat top,
         jfloat right, jfloat bottom) {
    if (handler) {
        return getHandler(handler)->cropCanvas(left, top, right, bottom).code;
    }
    return Hw::FAILED.code;
}

JNIEXPORT jint JNICALL Java_com_lmy_hwvcnative_processor_AlImageProcessor_cancelCropLayer
        (JNIEnv *env, jobject thiz, jlong handler, jint id) {
    if (handler) {
        return getHandler(handler)->cancelCropLayer(id).code;
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

JNIEXPORT jint JNICALL Java_com_lmy_hwvcnative_processor_AlImageProcessor_export
        (JNIEnv *env, jobject thiz, jlong handler, jstring path) {
    HwResult ret = Hw::FAILED.code;
    if (handler) {
        const char *pPath = env->GetStringUTFChars(path, JNI_FALSE);
        ret = getHandler(handler)->exportFile(std::string(pPath));
        env->ReleaseStringUTFChars(path, pPath);
    }
    return ret.code;
}

JNIEXPORT jint JNICALL Java_com_lmy_hwvcnative_processor_AlImageProcessor_import
        (JNIEnv *env, jobject thiz, jlong handler, jstring path) {
    HwResult ret = Hw::FAILED.code;
    if (handler) {
        const char *pPath = env->GetStringUTFChars(path, JNI_FALSE);
        ret = getHandler(handler)->importFile(std::string(pPath));
        env->ReleaseStringUTFChars(path, pPath);
    }
    return ret.code;
}

JNIEXPORT jint JNICALL Java_com_lmy_hwvcnative_processor_AlImageProcessor_ensureAlignCrop
        (JNIEnv *env, jobject thiz, jlong handler, jint id, jint rNum, jint rDen) {
    if (handler) {
        return getHandler(handler)->ensureAlignCrop(id, AlRational(rNum, rDen)).code;
    }
    return Hw::FAILED.code;
}

JNIEXPORT jint JNICALL Java_com_lmy_hwvcnative_processor_AlImageProcessor_cancelAlignCrop
        (JNIEnv *env, jobject thiz, jlong handler, jint id) {
    if (handler) {
        return getHandler(handler)->cancelAlignCrop(id).code;
    }
    return Hw::FAILED.code;
}

JNIEXPORT jint JNICALL Java_com_lmy_hwvcnative_processor_AlImageProcessor_redo
        (JNIEnv *env, jobject thiz, jlong handler) {
    if (handler) {
        return getHandler(handler)->redo().code;
    }
    return Hw::FAILED.code;
}

JNIEXPORT jint JNICALL Java_com_lmy_hwvcnative_processor_AlImageProcessor_undo
        (JNIEnv *env, jobject thiz, jlong handler) {
    if (handler) {
        return getHandler(handler)->undo().code;
    }
    return Hw::FAILED.code;
}

JNIEXPORT jint JNICALL Java_com_lmy_hwvcnative_processor_AlImageProcessor_paint
        (JNIEnv *env, jobject thiz, jlong handler, jint id, jfloat x, jfloat y, jboolean painting) {
    if (handler) {
        return getHandler(handler)->paint(id, x, y, JNI_TRUE == painting).code;
    }
    return Hw::FAILED.code;
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AlImageProcessor_queryLayerInfo
        (JNIEnv *env, jobject thiz, jlong handler) {
    if (handler) {
        getHandler(handler)->queryLayerInfo();
    }
}

#ifdef __cplusplus
}
#endif

