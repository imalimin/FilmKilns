/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICCONTEXT_H
#define FK_GRAPHIC_FKGRAPHICCONTEXT_H

#include "FkObject.h"
#include "FkGraphicWindow.h"
#include <EGL/egl.h>

FK_CLASS FkGraphicContext FK_EXTEND FkObject {
public:
    FkGraphicContext(const char *alias);

    FkGraphicContext(const std::string alias);

    FkGraphicContext(const FkGraphicContext &o) = delete;

    virtual ~FkGraphicContext();

    FkResult makeCurrent();

    FkResult create(std::shared_ptr<FkGraphicContext> context = nullptr,
                    std::shared_ptr<FkGraphicWindow> win = nullptr);

    FkResult update(std::shared_ptr<FkGraphicWindow> win);

    void destroy();

private:
    EGLDisplay _createDisplay(EGLNativeDisplayType display_id);

    EGLConfig _createConfig(EGLDisplay display, const int *configSpec);

    EGLContext _createContext(EGLDisplay display, EGLConfig config, EGLContext sharedContext);

    EGLSurface _createPbufferSurface(EGLDisplay display, EGLConfig config);

    EGLSurface _createWindowSurface(EGLDisplay display, EGLConfig config, std::shared_ptr<FkGraphicWindow> win);

    bool _checkError();

private:
    std::string alias;
    EGLDisplay eglDisplay = EGL_NO_DISPLAY;
    EGLConfig eglConfig = nullptr;
    EGLContext eglContext = EGL_NO_CONTEXT;
    EGLSurface eglSurface = EGL_NO_SURFACE;
};


#endif //FK_GRAPHIC_FKGRAPHICCONTEXT_H
