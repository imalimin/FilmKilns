/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicRenderAtom.h"
#include "FkGraphicLayerPrt.h"
#include "FkRenderRequestPrt.h"
#include "FkGraphicCtxComponent.h"
#include "FkGraphicTexComponent.h"
#include "FkGraphicFBOComponent.h"
#include "FkGraphicProgramComponent.h"
#include "FkSizeComponent.h"
#include "FkColorComponent.h"
#include "FkGLDefinition.h"

FkGraphicRenderAtom::FkGraphicRenderAtom() : FkSimpleAtom() {
    FK_MARK_SUPER

}

FkGraphicRenderAtom::~FkGraphicRenderAtom() {

}

void FkGraphicRenderAtom::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderRequestPrt, FkGraphicRenderAtom::_onRenderRequest);
}

void FkGraphicRenderAtom::onConnect(std::shared_ptr<FkConnectChain> chain) {
}

FkResult FkGraphicRenderAtom::onCreate() {
    return FkSimpleAtom::onCreate();
}

FkResult FkGraphicRenderAtom::onDestroy() {
    return FkSimpleAtom::onDestroy();
}

FkResult FkGraphicRenderAtom::onStart() {
    return FkSimpleAtom::onStart();
}

FkResult FkGraphicRenderAtom::onStop() {
    return FkSimpleAtom::onStop();
}

FkResult FkGraphicRenderAtom::_onRenderRequest(std::shared_ptr<FkProtocol> p) {
    auto prt = Fk_POINTER_CAST(FkRenderRequestPrt, p);
    std::vector<std::shared_ptr<FkGraphicComponent>> vec;
    std::shared_ptr<FkGraphicCtxComponent> context = nullptr;
    if (FK_OK != prt->req->findComponent(vec, FkClassType::type<FkGraphicCtxComponent>())) {
        return FK_FAIL;
    }
    context = Fk_POINTER_CAST(FkGraphicCtxComponent, vec[0]);
    vec.clear();
    std::shared_ptr<FkGraphicFBOComponent> fbo = nullptr;
    if (FK_OK != prt->req->findComponent(vec, FkClassType::type<FkGraphicFBOComponent>())) {
        return FK_FAIL;
    }
    fbo = Fk_POINTER_CAST(FkGraphicFBOComponent, vec[0]);
    vec.clear();
    std::shared_ptr<FkGraphicProgramComponent> program = nullptr;
    if (FK_OK != prt->req->findComponent(vec, FkClassType::type<FkGraphicProgramComponent>())) {
        return FK_FAIL;
    }
    program = Fk_POINTER_CAST(FkGraphicProgramComponent, vec[0]);
    for (auto &it : prt->req->layers) {
        vec.clear();
        std::shared_ptr<FkGraphicTexComponent> tex = nullptr;
        std::shared_ptr<FkSizeComponent> size = nullptr;
        if (FK_OK != it->findComponent(vec, FkClassType::type<FkGraphicTexComponent>())) {
            continue;
        }
        tex = Fk_POINTER_CAST(FkGraphicTexComponent, vec[0]);
        vec.clear();
        if (FK_OK != it->findComponent(vec, FkClassType::type<FkSizeComponent>())) {
            continue;
        }
        size = Fk_POINTER_CAST(FkSizeComponent, vec[0]);
        vec.clear();
        std::shared_ptr<FkColorComponent> color = nullptr;
        if (FK_OK != prt->req->layers[0]->findComponent(vec, FkClassType::type<FkColorComponent>())) {
            color = std::make_shared<FkColorComponent>();
            color->color = FkColor::white();
        }
        color = Fk_POINTER_CAST(FkColorComponent, vec[0]);
        vec.clear();

        fbo->fbo->attach(tex->tex);
        glClearColor(color->color.fRed(), color->color.fGreen(), color->color.fBlue(), 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        fbo->fbo->unbind();
    }
    return FK_OK;
}
