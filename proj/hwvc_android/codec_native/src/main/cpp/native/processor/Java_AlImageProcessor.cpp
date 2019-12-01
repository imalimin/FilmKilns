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

static AlImageProcessor *getHandler(jlong handler) {
    return reinterpret_cast<AlImageProcessor *>(handler);
}

JNIEXPORT jlong JNICALL Java_com_lmy_hwvcnative_processor_AlImageProcessor_create
        (JNIEnv *env, jobject thiz) {
    AlImageProcessor *p = new AlImageProcessor();
    p->post([] {
        HwJavaNativeHelper::getInstance()->attachThread();
    });
    jlong handler = reinterpret_cast<jlong>(p);
    HwJavaNativeHelper::getInstance()->registerAnObject(env, handler, thiz);
    return handler;
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AlImageProcessor_prepare
        (JNIEnv *env, jobject thiz, jlong handler, jobject surface, jstring path,
         jint width, jint height) {
    if (handler) {
        getHandler(handler)->prepare(new HwAndroidWindow(env, surface));
    }
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
        getHandler(handler)->moveLayerIndex(id, index).code;
    }
    return Hw::FAILED.code;
}

JNIEXPORT jint JNICALL Java_com_lmy_hwvcnative_processor_AlImageProcessor_removeLayer
        (JNIEnv *env, jobject thiz, jlong handler, jint id) {
    if (handler) {
        getHandler(handler)->removeLayer(id).code;
    }
    return Hw::FAILED.code;
}

JNIEXPORT jint JNICALL Java_com_lmy_hwvcnative_processor_AlImageProcessor_setScale
        (JNIEnv *env, jobject thiz, jlong handler, jint id, jfloat scale) {
    if (handler) {
        getHandler(handler)->setScale(id, scale).code;
    }
    return Hw::FAILED.code;
}

JNIEXPORT jint JNICALL Java_com_lmy_hwvcnative_processor_AlImageProcessor_setRotation
        (JNIEnv *env, jobject thiz, jlong handler, jint id, jfloat rotation) {
    if (handler) {
        getHandler(handler)->setRotation(id, rotation).code;
    }
    return Hw::FAILED.code;
}

JNIEXPORT jint JNICALL Java_com_lmy_hwvcnative_processor_AlImageProcessor_setTranslate
        (JNIEnv *env, jobject thiz, jlong handler, jint id, jfloat x, jfloat y) {
    if (handler) {
        getHandler(handler)->setTranslate(id, x, y);
    }
    return Hw::FAILED.code;
}

JNIEXPORT jint JNICALL Java_com_lmy_hwvcnative_processor_AlImageProcessor_postTranslate
        (JNIEnv *env, jobject thiz, jlong handler, jint id, jfloat dx, jfloat dy) {
    if (handler) {
        getHandler(handler)->postTranslate(id, dx, dy);
    }
    return Hw::FAILED.code;
}

JNIEXPORT jint JNICALL Java_com_lmy_hwvcnative_processor_AlImageProcessor_setAlpha
        (JNIEnv *env, jobject thiz, jlong handler, jint id, jfloat alpha) {
    if (handler) {
        getHandler(handler)->setAlpha(id, alpha).code;
    }
    return Hw::FAILED.code;
}

#ifdef __cplusplus
}
#endif

