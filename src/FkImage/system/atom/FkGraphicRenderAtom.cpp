/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicRenderAtom.h"
#include "FkRenderRequestPrt.h"
#include "FkPreRenderQuark.h"
#include "FkLayerRenderQuark.h"
#include "FkEmptyQuark.h"
#include "FkDeviceEntity.h"
#include "FkRenderContext.h"
#include "FkTransComponent.h"
#include "FkScaleComponent.h"
#include "FkRotateComponent.h"

FkGraphicRenderAtom::FkGraphicRenderAtom() : FkSimpleAtom() {
    FK_MARK_SUPER
}

FkGraphicRenderAtom::~FkGraphicRenderAtom() {

}

void FkGraphicRenderAtom::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderRequestPrt, FkGraphicRenderAtom::_onRenderRequest);
}

void FkGraphicRenderAtom::onConnect(std::shared_ptr<FkConnectChain> chain) {
    chain->next<FkEmptyQuark>();
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
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkRenderRequestPrt, p);
    auto renderEngine = FkRenderContext::wrap(getContext())->getRenderEngine();
    FkAssert(renderEngine != nullptr, nullptr);
    auto canvas = proto->req->getCanvas();
    for (auto &layer : proto->req->layers) {
        if (Fk_CANVAS_ID == layer->id) {
            continue;
        }
        auto transEntity = _makeTransEntity(canvas);
        std::shared_ptr<FkDeviceEntity> device = std::make_shared<FkTexDeviceEntity>(canvas->material);
        auto ret = renderEngine->renderDeviceWithTrans(layer->material, transEntity, device);
        FkAssert(FK_OK == ret, ret);
    }
    return _drawCanvas2Screen(canvas);
}

std::shared_ptr<FkTransEntity>
FkGraphicRenderAtom::_makeTransEntity(std::shared_ptr<FkGraphicLayer> &layer) {
    auto trans = std::make_shared<FkTransEntity>();
    trans->addComponent(layer->findComponent<FkTransComponent>());
    trans->addComponent(layer->findComponent<FkScaleComponent>());
    trans->addComponent(layer->findComponent<FkRotateComponent>());
    return trans;
}

FkResult FkGraphicRenderAtom::_drawCanvas2Screen(std::shared_ptr<FkGraphicLayer> &canvas) {
    auto renderEngine = FkRenderContext::wrap(getContext())->getRenderEngine();
    FkAssert(renderEngine != nullptr, nullptr);
    auto transEntity = _makeTransEntity(canvas);
    std::shared_ptr<FkDeviceEntity> device = std::make_shared<FkScreenEntity>();
    auto ret = renderEngine->renderDeviceWithTrans(canvas->material, transEntity, device);
    FkAssert(FK_OK == ret, ret);
    return ret;
}
