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
#include "FkTexComponent.h"
#include "FkQuerySizeProto.h"
#include "FkTransComponent.h"
#include "FkScaleComponent.h"
#include "FkRotateComponent.h"
#include "FkScaleTypeComponent.h"
#include "FkMeasureTransProto.h"

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
}

FkResult FkGraphicCanvasQuark::onCreate() {
    canvas = std::make_shared<FkGraphicLayer>();
    canvas->addComponent(std::make_shared<FkTransComponent>());
    canvas->addComponent(std::make_shared<FkScaleComponent>());
    canvas->addComponent(std::make_shared<FkRotateComponent>());
    auto scaleType = std::make_shared<FkScaleTypeComponent>();
    scaleType->value = kScaleType::CENTER_INSIDE;
    canvas->addComponent(scaleType);
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
            scaleComp->value.x = _getViewScale(canvas, proto->winSize);
            scaleComp->value.y = scaleComp->value.x;
            scaleComp->value.z = 1.0f;
        }
    }
    auto tesComp = proto->layer->findComponent<FkTexComponent>();
    if (nullptr != tesComp) {
        canvas->addComponent(tesComp);
    }
    return FK_OK;
}

FkResult FkGraphicCanvasQuark::_onRenderRequest(std::shared_ptr<FkProtocol> p) {
    auto prt = std::static_pointer_cast<FkRenderRequestPrt>(p);
    if (nullptr == prt->req) {
        prt->req = std::make_shared<FkRenderRequest>();
    }
    prt->req->canvas = std::make_shared<FkGraphicLayer>(*canvas);
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

float FkGraphicCanvasQuark::_getViewScale(std::shared_ptr<FkGraphicLayer> layer, FkSize &targetSize) {
    float scale = 1.0f;
    auto scaleType = layer->findComponent<FkScaleTypeComponent>();
    FkAssert(nullptr != scaleType, scale);
    auto size = layer->findComponent<FkSizeComponent>();
    FkAssert(nullptr != size, scale);
    auto &layerSize = size->size;
    switch (scaleType->value) {
        case kScaleType::CENTER_MATRIX:
            scale = 1.0f;
            break;
        case kScaleType::CENTER_INSIDE:
            scale = std::min(targetSize.getWidth() * 1.0f / layerSize.getWidth(),
                             targetSize.getHeight() * 1.0f / layerSize.getHeight());
            break;
        case kScaleType::CENTER_CROP:
            scale = std::max(targetSize.getWidth() * 1.0f / layerSize.getWidth(),
                             targetSize.getHeight() * 1.0f / layerSize.getHeight());
            break;
    }
    return scale;
}
