/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicContextQuark.h"
#include "FkGraphicNewTexPtl.h"
#include "FkSetSurfacePrt.h"
#include "FkRenderRequestPrt.h"

#define TAG "FkGraphicContextQuark"

const std::string FkGraphicContextQuark::FK_DETACH_CONTEXT = "EGLDetach";
const std::string FkGraphicContextQuark::FK_ATTACH_CONTEXT = "EGLAttach";

FkGraphicContextQuark::FkGraphicContextQuark() : FkQuark() {
    FK_MARK_SUPER
}

FkGraphicContextQuark::~FkGraphicContextQuark() {

}

void FkGraphicContextQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicNewTexPtl, FkGraphicContextQuark::_onMakeCurrent);
    FK_PORT_DESC_QUICK_ADD(desc, FkSetSurfacePrt, FkGraphicContextQuark::_onSetSurface);
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderRequestPrt, FkGraphicContextQuark::_onRenderRequest);
}

FkResult FkGraphicContextQuark::onCreate() {
    auto ret = FkQuark::onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    context = std::make_shared<FkGraphicContext>(FK_DETACH_CONTEXT);
    context->create();
    return ret;
}

FkResult FkGraphicContextQuark::onDestroy() {
    if (contextOfWin) {
        contextOfWin->destroy();
        contextOfWin = nullptr;
    }
    context->destroy();
    return FkQuark::onDestroy();
}

FkResult FkGraphicContextQuark::onStart() {
    return FkQuark::onStart();
}

FkResult FkGraphicContextQuark::onStop() {
    return FkQuark::onStop();
}

FkResult FkGraphicContextQuark::_onMakeCurrent(std::shared_ptr<FkProtocol> p) {
    auto ctx = (nullptr != contextOfWin ? contextOfWin : context);
    ctx->makeCurrent();
    if (FK_INSTANCE_OF(p, FkGraphicNewTexPtl)) {
        auto prt = Fk_POINTER_CAST(FkGraphicNewTexPtl, p);
        auto comp = std::make_shared<FkGraphicCtxComponent>();
        comp->context = ctx;
//        prt->layer->addComponent(comp);
    }
    return FK_OK;
}

FkResult FkGraphicContextQuark::_onSetSurface(std::shared_ptr<FkProtocol> p) {
    auto ptl = std::static_pointer_cast<FkSetSurfacePrt>(p);
    if (nullptr == contextOfWin) {
        contextOfWin = std::make_shared<FkGraphicContext>(FK_ATTACH_CONTEXT);
        return contextOfWin->create(context, ptl->win);
    } else {
        return contextOfWin->update(ptl->win);
    }
}

FkResult FkGraphicContextQuark::_onRenderRequest(std::shared_ptr<FkProtocol> p) {
    auto prt = std::static_pointer_cast<FkRenderRequestPrt>(p);
    auto ctx = (nullptr != contextOfWin ? contextOfWin : context);
    ctx->makeCurrent();
    auto comp = std::make_shared<FkGraphicCtxComponent>();
    comp->context = ctx;
    prt->req->addComponent(comp);
    return FK_OK;
}
