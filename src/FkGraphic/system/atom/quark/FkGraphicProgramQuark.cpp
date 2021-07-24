/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicProgramQuark.h"
#include "FkGraphicLayerPrt.h"
#include "FkGraphicProgramComponent.h"
#include "FkRenderRequestPrt.h"
#include "FkGraphicCtxComponent.h"

FkGraphicProgramQuark::FkGraphicProgramQuark() : FkQuark() {
    FK_MARK_SUPER

}

FkGraphicProgramQuark::~FkGraphicProgramQuark() {

}

void FkGraphicProgramQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderRequestPrt, FkGraphicProgramQuark::_onRenderRequest);
}

FkResult FkGraphicProgramQuark::onCreate() {
    auto ret = FkQuark::onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    allocator = std::make_shared<FkGraphicProgramAllocator>();
    return ret;
}

FkResult FkGraphicProgramQuark::onDestroy() {
    allocator->release();
    return FkQuark::onDestroy();
}

FkResult FkGraphicProgramQuark::onStart() {
    return FkQuark::onStart();
}

FkResult FkGraphicProgramQuark::onStop() {
    return FkQuark::onStop();
}

FkResult FkGraphicProgramQuark::_onRenderRequest(std::shared_ptr<FkProtocol> p) {
    auto proto = Fk_POINTER_CAST(FkRenderRequestPrt, p);
    std::vector<std::shared_ptr<FkGraphicComponent>> vec;
    vec.clear();
    if (FK_OK != proto->req->findComponent(vec, FkClassType::type<FkGraphicCtxComponent>())) {
        return FK_FAIL;
    }
    auto context = Fk_POINTER_CAST(FkGraphicCtxComponent, vec[0]);
    context->context->makeCurrent();

    auto comp = std::make_shared<FkGraphicProgramComponent>();
    FkProgramDescription desc(FkProgramDescription::kType::MATRIX);
    comp->program = allocator->alloc(desc);
    _fillValue(comp->program);
    proto->req->addComponent(comp);

    auto canvas = proto->req->getCanvas();
    FkAssert(nullptr != canvas, FK_FAIL);
    FkProgramDescription descMosaic(FkProgramDescription::kType::CANVAS_MOSAIC);
    auto canvasProgramComp = std::make_shared<FkGraphicProgramComponent>();
    canvasProgramComp->program = allocator->alloc(descMosaic);
    _fillValue(canvasProgramComp->program);
    canvas->addComponent(canvasProgramComp);
    return FK_OK;
}

FkResult FkGraphicProgramQuark::_fillValue(std::shared_ptr<FkGraphicProgram> program) {
    return FK_OK;
}
