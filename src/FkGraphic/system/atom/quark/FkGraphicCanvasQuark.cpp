/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicCanvasQuark.h"
#include "FkRenderRequestPrt.h"
#include "FkGraphicUpdateCanvasProto.h"
#include "FkSizeComponent.h"
#include "FkTexIDComponent.h"
#include "FkQuerySizeProto.h"
#include "FkTransComponent.h"
#include "FkScaleComponent.h"
#include "FkRotateComponent.h"
#include "FkScaleTypeComponent.h"
#include "FkMeasureTransProto.h"
#include "FkGraphicUpdateLayerPrt.h"
#include "FkDrawPointProto.h"
#include "FkPointComponent.h"

FkGraphicCanvasQuark::FkGraphicCanvasQuark() : FkQuark() {
    FK_MARK_SUPER
}

FkGraphicCanvasQuark::~FkGraphicCanvasQuark() {

}

void FkGraphicCanvasQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderRequestPrt, FkGraphicCanvasQuark::_onRenderRequest);
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicUpdateCanvasProto, FkGraphicCanvasQuark::_onUpdate);
    FK_PORT_DESC_QUICK_ADD(desc, FkQuerySizeProto, FkGraphicCanvasQuark::_onQueryCanvasSize);
    FK_PORT_DESC_QUICK_ADD(desc, FkMeasureTransProto, FkGraphicCanvasQuark::_onMeasureTrans);
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicUpdateLayerPrt, FkGraphicCanvasQuark::_onWithCanvasSize);
    FK_PORT_DESC_QUICK_ADD(desc, FkDrawPointProto, FkGraphicCanvasQuark::_onDrawPoint);
}

FkResult FkGraphicCanvasQuark::onCreate() {
    canvas = std::make_shared<FkGraphicLayer>();
    canvas->id = Fk_CANVAS_ID;
    canvas->addComponent(std::make_shared<FkTransComponent>());
    canvas->addComponent(std::make_shared<FkScaleComponent>());
    canvas->addComponent(std::make_shared<FkRotateComponent>());
    return FkQuark::onCreate();
}

FkResult FkGraphicCanvasQuark::onDestroy() {
    return FkQuark::onDestroy();
}

FkResult FkGraphicCanvasQuark::onStart() {
    return FkQuark::onStart();
}

FkResult FkGraphicCanvasQuark::onStop() {
    return FkQuark::onStop();
}

FkResult FkGraphicCanvasQuark::_onUpdate(std::shared_ptr<FkProtocol> p) {
    auto proto = Fk_POINTER_CAST(FkGraphicUpdateCanvasProto, p);
    auto sizeComp = proto->layer->findComponent<FkSizeComponent>();
    if (nullptr != sizeComp) {
        auto canvasSize = canvas->findComponent<FkSizeComponent>();
        if (nullptr != canvasSize) {
            canvasSize->size = sizeComp->size;
        } else {
            canvas->addComponent(sizeComp);
            auto scaleComp = canvas->findComponent<FkScaleComponent>();
            FkAssert(nullptr != scaleComp, scale);
            scaleComp->value.x = FkGraphicLayer::calcScaleWithScaleType(canvas, proto->scaleType, proto->winSize);
            scaleComp->value.y = scaleComp->value.x;
            scaleComp->value.z = 1.0f;
        }
    }
    auto texIDComp = proto->layer->findComponent<FkTexIDComponent>();
    if (nullptr != texIDComp) {
        canvas->addComponent(texIDComp);
    }
    return FK_OK;
}

FkResult FkGraphicCanvasQuark::_onRenderRequest(std::shared_ptr<FkProtocol> p) {
    auto prt = std::static_pointer_cast<FkRenderRequestPrt>(p);
    FkAssert(nullptr != prt->req, FK_EMPTY_DATA);
    prt->req->layers.emplace_back(std::make_shared<FkGraphicLayer>(*canvas));
    return FK_OK;
}

FkResult FkGraphicCanvasQuark::_onQueryCanvasSize(std::shared_ptr<FkProtocol> p) {
    auto proto = std::static_pointer_cast<FkQuerySizeProto>(p);
    std::vector<std::shared_ptr<FkGraphicComponent>> vec;
    if (FK_OK == canvas->findComponent(vec, FkClassType::type<FkSizeComponent>())) {
        auto sizeComp = Fk_POINTER_CAST(FkSizeComponent, vec[0]);
        proto->value = sizeComp->size;
    }
    vec.clear();
    return FK_OK;
}

FkResult FkGraphicCanvasQuark::_onMeasureTrans(std::shared_ptr<FkProtocol> p) {
    auto proto = Fk_POINTER_CAST(FkMeasureTransProto, p);
    proto->canvas = std::make_shared<FkGraphicLayer>(*canvas);
    return FK_OK;
}

FkResult FkGraphicCanvasQuark::_onWithCanvasSize(std::shared_ptr<FkProtocol> p) {
    auto proto = Fk_POINTER_CAST(FkGraphicUpdateLayerPrt, p);
    auto canvasSize = canvas->findComponent<FkSizeComponent>();
    if (nullptr != canvasSize) {
        proto->winSize = canvasSize->size;
    }
    return FK_OK;
}

FkResult FkGraphicCanvasQuark::_onDrawPoint(std::shared_ptr<FkProtocol> p) {
    auto proto = Fk_POINTER_CAST(FkDrawPointProto, p);
    if (proto->layer == canvas->id) {
        auto comp = std::make_shared<FkPointComponent>();
        comp->value = proto->value;
        comp->color = proto->color;
        canvas->addComponent(comp);
    }
    return FK_OK;
}
