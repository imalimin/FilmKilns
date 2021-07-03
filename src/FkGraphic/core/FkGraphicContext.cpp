/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicContext.h"

#define TAG "FkGraphicContext"

const int FK_CONFIG_WIN[] = {EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
                             EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                             EGL_RED_SIZE, 8,
                             EGL_GREEN_SIZE, 8,
                             EGL_BLUE_SIZE, 8,
                             EGL_ALPHA_SIZE, 8,
                             EGL_DEPTH_SIZE, 0,
                             EGL_STENCIL_SIZE, 0,
                             EGL_NONE};
const int FK_CONFIG_BUFFER[] = {EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
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
const int FK_CONFIG_ANDROID[] = {EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
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

FkGraphicContext::FkGraphicContext(const char *alias) : FkObject(), alias(alias) {
    FK_MARK_SUPER
}

FkGraphicContext::FkGraphicContext(const std::string alias) : FkObject(), alias(alias) {
    FK_MARK_SUPER
}

FkGraphicContext::~FkGraphicContext() {

}

FkResult FkGraphicContext::create(std::shared_ptr<FkGraphicContext> context,
                                  std::shared_ptr<FkGraphicWindow> win) {
    eglDisplay = _createDisplay(EGL_DEFAULT_DISPLAY);
    if (EGL_NO_DISPLAY == eglDisplay) {
        FkLogE(TAG, "[%s] Create display failed", alias.c_str());
        return FK_FAIL;
    }
    if (nullptr != win) {
        eglConfig = _createConfig(eglDisplay, FK_CONFIG_WIN);
    } else {
        eglConfig = _createConfig(eglDisplay, FK_CONFIG_BUFFER);
    }
    if (!eglConfig) {
        FkLogE(TAG, "[%s] Bad config", alias.c_str());
        return FK_FAIL;
    }
    if (nullptr != win) {
        size = win->getSize();
        eglSurface = _createWindowSurface(eglDisplay, eglConfig, win);
    } else {
        eglSurface = _createPbufferSurface(eglDisplay, eglConfig);
    }
    if (EGL_NO_SURFACE == eglSurface) {
        FkLogE(TAG, "[%s] Bad surface", alias.c_str());
        return FK_FAIL;
    }
    if (nullptr != context) {
        eglContext = _createContext(eglDisplay, eglConfig, context->eglContext);
    } else {
        eglContext = _createContext(eglDisplay, eglConfig, EGL_NO_CONTEXT);
    }
    FkLogI(TAG, "[%s] Create context %p", alias.c_str(), eglContext);
    if (EGL_NO_CONTEXT == eglContext) {
        FkLogE(TAG, "[%s] Bad context", alias.c_str());
        return FK_FAIL;
    }
    return FK_OK;
}

void FkGraphicContext::destroy() {
    if (eglDisplay != EGL_NO_DISPLAY) {
        if (!eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT)) {
            _checkError();
            FkLogE(TAG, "[%s] makeCurrent failed", alias.c_str());
        }
        if (eglSurface == EGL_NO_SURFACE || EGL_TRUE != eglDestroySurface(eglDisplay, eglSurface)) {
            FkLogE(TAG, "[%s] eglDestroySurface failed", alias.c_str());
        }
        if (eglContext == EGL_NO_CONTEXT || EGL_TRUE != eglDestroyContext(eglDisplay, eglContext)) {
            FkLogE(TAG, "[%s] eglDestroyContext failed", alias.c_str());
        }
        if (EGL_TRUE != eglTerminate(eglDisplay)) {
            FkLogE(TAG, "[%s] eglTerminate failed", alias.c_str());
        }
    }
    eglContext = EGL_NO_CONTEXT;
    eglSurface = EGL_NO_SURFACE;
    eglDisplay = EGL_NO_DISPLAY;
    eglConfig = nullptr;
}


EGLDisplay FkGraphicContext::_createDisplay(EGLNativeDisplayType display_id) {
    EGLDisplay display = eglGetDisplay(display_id);
    if (EGL_NO_DISPLAY == display || !_checkError()) {
        FkLogE(TAG, "[%s] eglGetDisplay failed", alias.c_str());
        return EGL_NO_DISPLAY;
    }
    EGLint majorVersion;
    EGLint minorVersion;
    if (!eglInitialize(display, // 创建的EGL连接
                       &majorVersion, // 返回EGL主板版本号
                       &minorVersion) || !_checkError()) { // 返回EGL次版本号
        FkLogE(TAG, "[%s] eglInitialize failed", alias.c_str());
        return EGL_NO_DISPLAY;
    }
    return display;
}

EGLConfig FkGraphicContext::_createConfig(EGLDisplay display, const int *configSpec) {
    EGLint configsCount = 0;
    const EGLint maxConfigs = 2;
    EGLConfig configs[maxConfigs];
    //Get a list of all EGL frame buffer configurations for a display
    EGLBoolean ret = eglGetConfigs(display, configs, maxConfigs, &configsCount);
    if (ret != EGL_TRUE || configsCount <= 0) {
        FkLogE(TAG, "[%s] eglChooseConfig failed", alias.c_str());
        return nullptr;
    }

    // Get a list of EGL frame buffer configurations that match specified attributes
    ret = eglChooseConfig(display, // 创建的和本地窗口系统的连接
                          configSpec, // 指定渲染表面的参数列表，可以为null
                          configs, // 调用成功，返会符合条件的EGLConfig列表
                          maxConfigs, // 最多返回的符合条件的EGLConfig个数
                          &configsCount); // 实际返回的符合条件的EGLConfig个数
    if (EGL_TRUE != ret || configsCount <= 0 || !_checkError()) {
        FkLogE(TAG, "[%s] eglChooseConfig failed", alias.c_str());
        return nullptr;
    }
    return configs[0];
}

EGLContext FkGraphicContext::_createContext(EGLDisplay display, EGLConfig config,
                                            EGLContext sharedContext) {
    int contextSpec[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
    EGLContext context = eglCreateContext(display, config, sharedContext, contextSpec);
    if (EGL_NO_CONTEXT == context || !_checkError()) {
        FkLogE(TAG, "[%s] eglCreateContext failed", alias.c_str());
        return EGL_NO_CONTEXT;
    }
    return context;
}

EGLSurface FkGraphicContext::_createPbufferSurface(EGLDisplay display, EGLConfig config) {
    int attrib_list[] = {EGL_WIDTH, 1, EGL_HEIGHT, 1, EGL_NONE};
    EGLSurface surface = eglCreatePbufferSurface(display, config, attrib_list);
    if (nullptr == surface || EGL_NO_SURFACE == surface || !_checkError()) {
        FkLogE(TAG, "[%s] eglCreatePbufferSurface failed", alias.c_str());
        return EGL_NO_SURFACE;
    }
    EGLBoolean ret = eglBindAPI(EGL_OPENGL_ES_API);
    if (EGL_TRUE != ret) {
        FkLogE(TAG, "[%s] eglBindAPI failed", alias.c_str());
        return EGL_NO_SURFACE;
    }
    return surface;
}

EGLSurface FkGraphicContext::_createWindowSurface(EGLDisplay display,
                                                  EGLConfig config,
                                                  std::shared_ptr<FkGraphicWindow> win) {
    int attrib_list[] = {EGL_NONE};
    auto surface = eglCreateWindowSurface(display,
                                          config, // 选好的可用EGLConfig
                                          win->getNativeWindow(), // 指定原生窗口
                                          attrib_list); // 指定窗口属性列表，可以为null，一般指定渲染所用的缓冲区使用但缓冲或者后台缓冲，默认为后者。
    if (nullptr == surface || EGL_NO_SURFACE == surface || !_checkError()) {
        FkLogE(TAG, "[%s] eglCreateWindowSurface failed", alias.c_str());
        return EGL_NO_SURFACE;
    }
    EGLBoolean ret = eglBindAPI(EGL_OPENGL_ES_API);
    if (EGL_TRUE != ret) {
        FkLogE(TAG, "[%s] eglBindAPI failed", alias.c_str());
        return EGL_NO_SURFACE;
    }
    return surface;

}

FkResult FkGraphicContext::makeCurrent() {
    if (EGL_NO_DISPLAY == eglDisplay) {
        FkLogE(TAG, "[%s] name egl failed had release!", alias.c_str());
        return FK_FAIL;
    }
    auto ret = eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
    if (EGL_TRUE != ret || !_checkError()) {
        FkLogE(TAG, "[%s] name makeCurrent failed", alias.c_str());
        return FK_FAIL;
    }
    return FK_OK;
}

FkResult FkGraphicContext::update(std::shared_ptr<FkGraphicWindow> win) {
    /// 只有初始化时带Surface的egl才能更新Surface
    if (win) {
        if (EGL_NO_SURFACE != eglSurface) {
            eglDestroySurface(eglDisplay, eglSurface);
            eglSurface = EGL_NO_SURFACE;
        }
        eglSurface = _createWindowSurface(eglDisplay, eglConfig, win);
        makeCurrent();
        AlLogE(TAG, "Update window");
        return FK_OK;
    }
    return FK_FAIL;
}

FkResult FkGraphicContext::swapBuffers() {
    if (!eglSwapBuffers(eglDisplay, eglSurface)) {
        AlLogE(TAG, "%s swapBuffers failed!", alias.c_str());
        return FK_FAIL;
    }
    return FK_OK;
}

bool FkGraphicContext::_checkError() {
    EGLint error = eglGetError();
    if (EGL_SUCCESS != error) {
        FkLogE(TAG, "[%s] Bad EGL environment: %d", alias.c_str(), error);
        return false;
    }
    return true;
}

int32_t FkGraphicContext::getWidth() {
    return size.getWidth();
}

int32_t FkGraphicContext::getHeight() {
    return size.getHeight();
}
