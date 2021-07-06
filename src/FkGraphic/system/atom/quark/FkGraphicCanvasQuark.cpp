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

FkGraphicCanvasQuark::FkGraphicCanvasQuark() : FkQuark() {
    FK_MARK_SUPER
}

FkGraphicCanvasQuark::~FkGraphicCanvasQuark() {

}

void FkGraphicCanvasQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderRequestPrt, FkGraphicCanvasQuark::_onRenderRequest);
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicUpdateCanvasProto, FkGraphicCanvasQuark::_onRenderRequest);
}

FkResult FkGraphicCanvasQuark::onCreate() {
    canvas = std::make_shared<FkGraphicLayer>();
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
    auto proto = std::static_pointer_cast<FkGraphicUpdateCanvasProto>(p);
    std::vector<std::shared_ptr<FkGraphicComponent>> vec;
    if (FK_OK == proto->layer->findComponent(vec, FkClassType::type<FkSizeComponent>())) {
        canvas->addComponent(vec[0]);
    }
    vec.clear();
    if (FK_OK == proto->layer->findComponent(vec, FkClassType::type<FkTexComponent>())) {
        canvas->addComponent(vec[0]);
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