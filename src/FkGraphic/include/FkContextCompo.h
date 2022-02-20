/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-20 23:37:39
*/

#ifndef FK_GRAPHIC_FKCONTEXTCOMPO_H
#define FK_GRAPHIC_FKCONTEXTCOMPO_H

#include "FkComponent.h"
#include "FkGraphicWindow.h"
#include <EGL/egl.h>

FK_CLASS FkContextCompo FK_EXTEND FkComponent {
public:
    FkContextCompo(const std::string alias);

    FkContextCompo(const FkContextCompo &o) = delete;

    virtual ~FkContextCompo();

    FkResult makeCurrent();

    FkResult create(std::shared_ptr<FkContextCompo> context = nullptr,
                    std::shared_ptr<FkGraphicWindow> win = nullptr);

    FkResult update(std::shared_ptr<FkGraphicWindow> win);

    void destroy();

    FkResult swapBuffers();

    int32_t getWidth();

    int32_t getHeight();

private:
    EGLDisplay _createDisplay(EGLNativeDisplayType display_id);

    EGLConfig _createConfig(EGLDisplay display, const int *configSpec);

    EGLContext _createContext(EGLDisplay display, EGLConfig config, EGLContext sharedContext);

    EGLSurface _createPbufferSurface(EGLDisplay display, EGLConfig config);

    EGLSurface _createWindowSurface(EGLDisplay display, EGLConfig config, std::shared_ptr<FkGraphicWindow> win);

    bool _checkError();

private:
    static const int FK_CONFIG_WIN[];
    static const int FK_CONFIG_BUFFER[];
#ifdef __ANDROID__
    static const int EGL_RECORDABLE_ANDROID;
    static const int FK_CONFIG_ANDROID[];
#endif
    std::string alias;
    FkSize size;
    EGLDisplay eglDisplay = EGL_NO_DISPLAY;
    EGLConfig eglConfig = nullptr;
    EGLContext eglContext = EGL_NO_CONTEXT;
    EGLSurface eglSurface = EGL_NO_SURFACE;
};

#endif //FK_GRAPHIC_FKCONTEXTCOMPO_H