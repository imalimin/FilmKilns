/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: ${author}
* CREATE TIME: 2022-3-1 13:20:29
*/

#include "FkGLEnvAtom.h"
#include "FkRenderProto.h"
#include "FkWindowProto.h"
#include "FkEmptyQuark.h"
#include "FkSizeCompo.h"
#include "FkOffWindow.h"
#include "FkGLDefinition.h"
#include "FkRenderContext.h"

FK_IMPL_CLASS_TYPE(FkGLEnvAtom, FkSimpleAtom)

FkGLEnvAtom::FkGLEnvAtom() : FkSimpleAtom(), glVersion(FK_GL_VER_2) {

}

FkGLEnvAtom::~FkGLEnvAtom() {

}

void FkGLEnvAtom::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderProto, FkGLEnvAtom::_onRender);
    FK_PORT_DESC_QUICK_ADD(desc, FkWindowProto, FkGLEnvAtom::_onUpdateWindow);
}

void FkGLEnvAtom::onConnect(std::shared_ptr<FkConnectChain> chain) {
    chain->next<FkEmptyQuark>();
}

FkResult FkGLEnvAtom::onCreate() {
    auto ret = FkSimpleAtom::onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    auto _context = FkRenderContext::wrap(getContext());
    FkAssert(_context != nullptr, FK_NPE);
    glVersion = _context->getRenderSettings()->getGlVersion();
    FkLogI(FK_DEF_TAG, "glVersion: %d", glVersion);
    _initializeWithoutWindow();
    context->makeCurrent();

    GLint values[2];
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, values);
    _context->getRenderSettings()->setMaxTextureSize(values[0]);
    glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, values);
    _context->getRenderSettings()->setMaxCountOfVertexTexture(values[0]);
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, values);
    _context->getRenderSettings()->setMaxCountOfFragmentTexture(values[0]);
    glGetIntegerv(GL_MAX_VIEWPORT_DIMS, values);
    _context->getRenderSettings()->setMaxViewportSize(values[0], values[1]);
    return ret;
}

FkResult FkGLEnvAtom::onDestroy() {
    auto ret = FkSimpleAtom::onDestroy();
    if (FK_OK != ret) {
        return ret;
    }
    _destroyContext();
    return ret;
}

FkResult FkGLEnvAtom::onStart() {
    auto ret = FkSimpleAtom::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    _initializeWithoutWindow();
    context->makeCurrent();
    return ret;
}

FkResult FkGLEnvAtom::onStop() {
    auto ret = FkSimpleAtom::onStop();
    if (FK_OK != ret) {
        return ret;
    }
    _destroyContext();
    return ret;
}
FkResult FkGLEnvAtom::_onRender(std::shared_ptr<FkProtocol> p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkRenderProto, p);
    if (FK_INSTANCE_OF(proto->device, FkScreenEntity)) {
        auto size = FkSize(context->getWidth(), context->getHeight());
        proto->device->addComponent(std::make_shared<FkSizeCompo>(size));
    }
    context->makeCurrent();
    proto->env->addComponent(context);
    return dispatchNext(p);
}

FkResult FkGLEnvAtom::_onUpdateWindow(std::shared_ptr<FkProtocol> p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkWindowProto, p);
    FkResult ret = FK_OK;
    if (context && !_isOffWin(proto->win)) {
        ret = _changeWithWindow(proto->win);
    } else if (context && _isOffWin(proto->win)) {
        ret = _changeWithoutWindow();
    } else if (context == nullptr && !_isOffWin(proto->win)) {
        _initializeWithWindow(proto->win);
    } else {
        _initializeWithoutWindow();
    }
    context->makeCurrent();
    if (FK_OK != ret) {
        return ret;
    }
    return dispatchNext(p);
}

FkResult FkGLEnvAtom::_changeWithWindow(std::shared_ptr<FkGraphicWindow> &win) {
    if (context->isPBuffer()) {
        auto newContext = std::make_shared<FkContextCompo>(glVersion, "EGLAttach");
        auto ret = newContext->create(context, win);
        context->destroy();
        context = newContext;
        return ret;
    } else {
        return context->update(win);
    }
}

FkResult FkGLEnvAtom::_changeWithoutWindow() {
    if (!context->isPBuffer()) {
        auto newContext = std::make_shared<FkContextCompo>(glVersion, "EGLDetach");
        auto ret = newContext->create(context, nullptr);
        context->destroy();
        context = newContext;
        return ret;
    }
    return FK_OK;
}

FkResult FkGLEnvAtom::_initializeWithWindow(std::shared_ptr<FkGraphicWindow> &win) {
    if (context == nullptr) {
        context = std::make_shared<FkContextCompo>(glVersion, "EGLAttach");
        return context->create(nullptr, win);
    }
    return FK_FAIL;
}

FkResult FkGLEnvAtom::_initializeWithoutWindow() {
    if (context == nullptr) {
        context = std::make_shared<FkContextCompo>(glVersion, "EGLDetach");
        return context->create();
    }
    return FK_FAIL;
}

FkResult FkGLEnvAtom::_destroyContext() {
    if (context) {
        context->destroy();
        context = nullptr;
        return FK_OK;
    }
    return FK_NPE;
}

bool FkGLEnvAtom::_isOffWin(std::shared_ptr<FkGraphicWindow> &win) {
    return win == nullptr || FK_INSTANCE_OF(win, FkOffWindow);
}
