/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkPreRenderQuark.h"
#include "FkRenderRequestPrt.h"
#include "FkGraphicCtxComponent.h"
#include "FkGraphicFBOComponent.h"
#include "FkGraphicProgramComponent.h"
#include "FkGraphicTexComponent.h"
#include "FkSizeComponent.h"
#include "FkVertexObjectComponent.h"
#include "FkGraphicRender.h"
#include "FkGLDefinition.h"

FkPreRenderQuark::FkPreRenderQuark() : FkQuark() {
    FK_MARK_SUPER
}

FkPreRenderQuark::~FkPreRenderQuark() {

}

void FkPreRenderQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderRequestPrt, FkPreRenderQuark::_onRenderRequest);
}

FkResult FkPreRenderQuark::onCreate() {
    return FkQuark::onCreate();
}

FkResult FkPreRenderQuark::onDestroy() {
    return FkQuark::onDestroy();
}

FkResult FkPreRenderQuark::onStart() {
    return FkQuark::onStart();
}

FkResult FkPreRenderQuark::onStop() {
    return FkQuark::onStop();
}

FkResult FkPreRenderQuark::_onRenderRequest(std::shared_ptr<FkProtocol> p) {
    auto proto = Fk_POINTER_CAST(FkRenderRequestPrt, p);
    auto canvas = proto->req->getCanvas();
    FkAssert(nullptr != canvas, FK_FAIL);
    std::vector<std::shared_ptr<FkGraphicComponent>> vec;
    vec.clear();
    if (FK_OK != canvas->findComponent(vec, FkClassType::type<FkGraphicProgramComponent>())) {
        return FK_SKIP;
    }
    auto program = Fk_POINTER_CAST(FkGraphicProgramComponent, vec[0]);

    vec.clear();
    if (FK_OK != proto->req->findComponent(vec, FkClassType::type<FkGraphicCtxComponent>())) {
        return FK_FAIL;
    }
    auto context = Fk_POINTER_CAST(FkGraphicCtxComponent, vec[0]);

    vec.clear();
    if (FK_OK != proto->req->findComponent(vec, FkClassType::type<FkGraphicFBOComponent>())) {
        return FK_FAIL;
    }
    auto fbo = Fk_POINTER_CAST(FkGraphicFBOComponent, vec[0]);

    vec.clear();
    if (FK_OK != canvas->findComponent(vec, FkClassType::type<FkGraphicTexComponent>())) {
        return FK_FAIL;
    }
    auto tex = Fk_POINTER_CAST(FkGraphicTexComponent, vec[0]);

    vec.clear();
    if (FK_OK != canvas->findComponent(vec, FkClassType::type<FkSizeComponent>())) {
        return FK_FAIL;
    }
    auto size = Fk_POINTER_CAST(FkSizeComponent, vec[0]);
//    auto verObj = canvas->findComponent<FkVertexObjectComponent>();
//    FkAssert(nullptr != verObj, FK_NPE);
//
//    vec.clear();
//    context->context->makeCurrent();
//    auto ret = FkGraphicRender::with(program->program)
//            ->setContext(context->context)
//            ->setViewport(0, 0, size->size.getWidth(), size->size.getHeight())
//            ->setVertexObj(verObj)
//            ->setFrameObject(fbo->fbo)
//            ->setTargetTexture(tex->tex)
//            ->render();
//    return ret;
    return FK_OK;
}