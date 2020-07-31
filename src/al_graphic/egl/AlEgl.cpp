/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlEgl.h"
#include "AlLogcat.h"

#ifdef __ANDROID__

#include <android/native_window_jni.h>

bool AlEgl::setTimestamp(int64_t nsecs) {
    if (nullptr == eglPresentationTimeANDROID || EGL_NO_DISPLAY == eglDisplay) {
        return false;
    }
    return EGL_TRUE == eglPresentationTimeANDROID(eglDisplay, eglSurface, nsecs);
}

#endif

#define TAG "AlEgl"

const int CONFIG_WIN[] = {EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
                          EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                          EGL_RED_SIZE, 8,
                          EGL_GREEN_SIZE, 8,
                          EGL_BLUE_SIZE, 8,
                          EGL_ALPHA_SIZE, 8,
                          EGL_DEPTH_SIZE, 0,
                          EGL_STENCIL_SIZE, 0,
                          EGL_NONE};
const int CONFIG_BUFFER[] = {EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
                             EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
                             EGL_RED_SIZE, 8,
                             EGL_GREEN_SIZE, 8,
                             EGL_BLUE_SIZE, 8,
                             EGL_ALPHA_SIZE, 8,
                             EGL_DEPTH_SIZE, 0,
                             EGL_STENCIL_SIZE, 0,
                             EGL_NONE};

#ifdef __ANDROID__
const int EGL_RECORDABLE_ANDROID = 0x3142;
const int CONFIG_ANDROID[] = {EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
                              EGL_RED_SIZE, 8,
                              EGL_GREEN_SIZE, 8,
                              EGL_BLUE_SIZE, 8,
                              EGL_ALPHA_SIZE, 8,
                              EGL_RECORDABLE_ANDROID, 1,
                              EGL_DEPTH_SIZE, 0,
                              EGL_STENCIL_SIZE, 0,
                              EGL_NONE
};
#endif

EGLContext AlEgl::currentContext() {
    EGLContext context = eglGetCurrentContext();
    if (EGL_NO_CONTEXT == context) {
        AlLogW(TAG, "Current thread has non context.");
    }
    return context;
}

AlEgl *AlEgl::offScreen(std::string alias, EGLContext context) {
    AlEgl *egl = new AlEgl(alias);
    egl->init(context, nullptr, CONFIG_BUFFER);
    return egl;
}

AlEgl *AlEgl::window(std::string alias, HwWindow *win, EGLContext context) {
    AlEgl *egl = new AlEgl(alias);
    if (win && win->getANativeWindow()) {
        egl->init(context, win, CONFIG_WIN);
    } else {
        egl->init(context, win, CONFIG_BUFFER);
    }
    return egl;
}

#ifdef __ANDROID__

AlEgl *AlEgl::androidCodec(std::string alias, HwWindow *win, EGLContext context) {
    if (nullptr == win || nullptr == win->getANativeWindow()) {
        return nullptr;
    }
    AlEgl *egl = new AlEgl(alias);
    egl->init(context, win, CONFIG_ANDROID);
    return egl;
}

#endif

AlEgl::AlEgl(const std::string &alias) : Object(), alias(alias) {

}

AlEgl::~AlEgl() {
    if (eglDisplay != EGL_NO_DISPLAY) {
        if (!eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT)) {
            checkError();
            AlLogE(TAG, "makeCurrent failed");
        }
        if (eglSurface == EGL_NO_SURFACE || EGL_TRUE != eglDestroySurface(eglDisplay, eglSurface)) {
            AlLogE(TAG, "eglDestroySurface failed");
        }
        if (eglContext == EGL_NO_CONTEXT || EGL_TRUE != eglDestroyContext(eglDisplay, eglContext)) {
            AlLogE(TAG, "eglDestroyContext failed");
        }
        if (EGL_TRUE != eglTerminate(eglDisplay)) {
            AlLogE(TAG, "eglTerminate failed");
        }
    }
    eglContext = EGL_NO_CONTEXT;
    eglSurface = EGL_NO_SURFACE;
    eglDisplay = EGL_NO_DISPLAY;
    eglConfig = nullptr;
    if (this->win) {
        delete this->win;
        this->win = nullptr;
    }
    AlLogI(TAG, "alias %s", alias.c_str());
}


void AlEgl::init(EGLContext context, HwWindow *win, const int *config) {
    if (EGL_NO_DISPLAY != eglDisplay
        || EGL_NO_SURFACE != eglContext
        || EGL_NO_SURFACE != eglSurface) {
        AlLogE(TAG, "Dirty env!!!!!!!!!!");
        return;
    }
    this->win = win;
    createDisplay(EGL_DEFAULT_DISPLAY);
    if (EGL_NO_DISPLAY == this->eglDisplay) {
        AlLogE(TAG, "failed");
        return;
    }
    createConfig(config);
    if (!this->eglConfig) {
        AlLogE(TAG, "Bad config");
        return;
    }
    if (this->win) {
        createWindowSurface(this->win);
    } else {
        createPbufferSurface();
    }
    if (EGL_NO_SURFACE == this->eglSurface) {
        AlLogE(TAG, "Bad surface");
        return;
    }
    if (context) {
        createContext(context);
    } else {
        createContext(EGL_NO_CONTEXT);
    }
    AlLogI(TAG, "Create context %p", this->eglContext);
    if (EGL_NO_CONTEXT == this->eglContext) {
        AlLogE(TAG, "Bad context");
        return;
    }
    makeCurrent();
    int width = 0, height = 0;
    if (!eglQuerySurface(eglDisplay, eglSurface, EGL_WIDTH, &width) ||
        !eglQuerySurface(eglDisplay, eglSurface, EGL_HEIGHT, &height)) {
        AlLogE(TAG, "Egl init failed");
    }

#ifdef __ANDROID__
    eglPresentationTimeANDROID = reinterpret_cast<EGL_PRESENTATION_TIME_ANDROID>(eglGetProcAddress(
            "eglPresentationTimeANDROID"));
#endif
    //If interval is set to a value of 0, buffer swaps are not synchronized to a video frame, and the swap happens as soon as the render is complete.
//    eglSwapInterval(eglDisplay, 0);
    AlLogI(TAG, "alias %s", alias.c_str());
}

EGLDisplay AlEgl::createDisplay(EGLNativeDisplayType display_id) {
    eglDisplay = eglGetDisplay(display_id);
    if (EGL_NO_DISPLAY == eglDisplay || !checkError()) {
        AlLogE(TAG, "eglGetDisplay failed");
        return EGL_NO_DISPLAY;
    }
    EGLint majorVersion;
    EGLint minorVersion;
    if (!eglInitialize(eglDisplay, // 创建的EGL连接
                       &majorVersion, // 返回EGL主板版本号
                       &minorVersion) || !checkError()) { // 返回EGL次版本号
        AlLogE(TAG, "eglInitialize failed");
        return EGL_NO_DISPLAY;
    }
    return eglDisplay;
}

EGLConfig AlEgl::createConfig(const int *configSpec) {
    EGLint configsCount = 0;
    const EGLint maxConfigs = 2;
    EGLConfig configs[maxConfigs];
    //Get a list of all EGL frame buffer configurations for a display
    EGLBoolean ret = eglGetConfigs(eglDisplay, configs, maxConfigs, &configsCount);
    if (ret != EGL_TRUE || configsCount <= 0) {
        AlLogE(TAG, "eglChooseConfig failed");
        return nullptr;
    }

    // Get a list of EGL frame buffer configurations that match specified attributes
    ret = eglChooseConfig(eglDisplay, // 创建的和本地窗口系统的连接
                          configSpec, // 指定渲染表面的参数列表，可以为null
                          configs, // 调用成功，返会符合条件的EGLConfig列表
                          maxConfigs, // 最多返回的符合条件的EGLConfig个数
                          &configsCount); // 实际返回的符合条件的EGLConfig个数
    if (EGL_TRUE != ret || configsCount <= 0 || !checkError()) {
        AlLogE(TAG, "eglChooseConfig failed");
        return nullptr;
    }
    eglConfig = configs[0];
    if (win && win->getANativeWindow()) {
        EGLint format;
        if (EGL_TRUE != eglGetConfigAttrib(eglDisplay, eglConfig, EGL_NATIVE_VISUAL_ID, &format)) {
            AlLogE(TAG, "eglGetConfigAttrib failed");
        } else {
            ANativeWindow_setBuffersGeometry(win->getANativeWindow(), 0, 0, format);
        }
    }
    return configs[0];
}

EGLContext AlEgl::createContext(EGLContext context) {
    int contextSpec[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
    eglContext = eglCreateContext(eglDisplay, eglConfig, context, contextSpec);
    if (EGL_NO_CONTEXT == eglContext || !checkError()) {
        AlLogE(TAG, "eglCreateContext failed");
        return EGL_NO_CONTEXT;
    }
    return eglContext;
}

EGLSurface AlEgl::createPbufferSurface() {
//    EGLint values;
//    eglQueryContext(eglDisplay, eglContext, EGL_CONTEXT_CLIENT_VERSION, &values);
    int attrib_list[] = {EGL_WIDTH, 1, EGL_HEIGHT, 1, EGL_NONE};
    eglSurface = eglCreatePbufferSurface(eglDisplay, eglConfig, attrib_list);
    if (nullptr == eglSurface || EGL_NO_SURFACE == eglSurface || !checkError()) {
        AlLogE(TAG, "eglCreatePbufferSurface failed");
        return EGL_NO_SURFACE;
    }
    EGLBoolean ret = eglBindAPI(EGL_OPENGL_ES_API);
    if (EGL_TRUE != ret) {
        AlLogE(TAG, "eglBindAPI failed");
        return EGL_NO_SURFACE;
    }
    return eglSurface;
}

EGLSurface AlEgl::createWindowSurface(HwWindow *win) {
//    EGLint values;
//    eglQueryContext(eglDisplay, eglContext, EGL_CONTEXT_CLIENT_VERSION, &values);
    int attrib_list[] = {EGL_NONE};
    eglSurface = eglCreateWindowSurface(eglDisplay,
                                        eglConfig, // 选好的可用EGLConfig
                                        win->getANativeWindow(), // 指定原生窗口
                                        attrib_list); // 指定窗口属性列表，可以为null，一般指定渲染所用的缓冲区使用但缓冲或者后台缓冲，默认为后者。
    if (nullptr == eglSurface || EGL_NO_SURFACE == eglSurface || !checkError()) {
        AlLogE(TAG, "eglCreateWindowSurface failed");
        return EGL_NO_SURFACE;
    }
    EGLBoolean ret = eglBindAPI(EGL_OPENGL_ES_API);
    if (EGL_TRUE != ret) {
        AlLogE(TAG, "eglBindAPI failed");
        return EGL_NO_SURFACE;
    }
    return eglSurface;
}

int AlEgl::width() {
    return getParams(EGL_WIDTH);
}

int AlEgl::height() {
    return getParams(EGL_HEIGHT);
}

EGLint AlEgl::getParams(EGLint attribute) {
    EGLint params;
    eglQuerySurface(eglDisplay, eglSurface, attribute, &params);
    return params;
}

void AlEgl::makeCurrent() {
    if (EGL_NO_DISPLAY == eglDisplay) {
        AlLogE(TAG, "name egl failed had release!");
        return;
    }
    if (!eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext) || !checkError()) {
        AlLogE(TAG, "name makeCurrent failed");
    }
}

void AlEgl::swapBuffers() {
    if (!eglSwapBuffers(eglDisplay, eglSurface)) {
        AlLogE(TAG, "%s swapBuffers failed!", alias.c_str());
    }
}

bool AlEgl::checkError() {
    EGLint error = eglGetError();
    if (EGL_SUCCESS != error) {
        AlLogE(TAG, "Bad EGL environment: %d", error);
        return false;
    }
    return true;
}

bool AlEgl::updateWindow(HwWindow *win) {
    /// 只有初始化时带Surface的egl才能更新Surface
    if (this->win) {
        delete this->win;
        this->win = nullptr;
        this->win = win;
        if (EGL_NO_SURFACE != eglSurface) {
            eglDestroySurface(eglDisplay, eglSurface);
            eglSurface = EGL_NO_SURFACE;
        }
        createWindowSurface(this->win);
        makeCurrent();
        AlLogE(TAG, "Update window");
        return true;
    }
    return false;
}

bool AlEgl::isAttachWindow() { return nullptr != win; }

EGLContext AlEgl::getContext() {
    return eglContext;
}