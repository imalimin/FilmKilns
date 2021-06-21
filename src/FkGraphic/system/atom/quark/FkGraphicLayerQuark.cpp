/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicLayerQuark.h"
#include "FkGraphicNewLayerPrt.h"
#include "FkGraphicUpdateLayerPrt.h"
#include "FkColorComponent.h"
#include "FkSizeComponent.h"
#include "FkTexComponent.h"
#include "FkRenderRequestPrt.h"

FkGraphicLayerQuark::FkGraphicLayerQuark() : FkQuark() {
    FK_MARK_SUPER
}

FkGraphicLayerQuark::~FkGraphicLayerQuark() {

}

void FkGraphicLayerQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicNewLayerPrt, FkGraphicLayerQuark::_onNewLayer);
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicUpdateLayerPrt, FkGraphicLayerQuark::_onUpdateLayer);
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderRequestPrt, FkGraphicLayerQuark::_onRenderRequest);
}

FkResult FkGraphicLayerQuark::onCreate() {
    return FkQuark::onCreate();
}

FkResult FkGraphicLayerQuark::onDestroy() {
    return FkQuark::onDestroy();
}

FkResult FkGraphicLayerQuark::onStart() {
    return FkQuark::onStart();
}

FkResult FkGraphicLayerQuark::onStop() {
    return FkQuark::onStop();
}

FkResult FkGraphicLayerQuark::_onNewLayer(std::shared_ptr<FkProtocol> p) {
    auto prt = std::static_pointer_cast<FkGraphicNewLayerPrt>(p);
    auto layer = std::make_shared<FkGraphicLayer>();
    std::lock_guard<std::mutex> guard(mtx);
    ++mCurID;
    layer->id = mCurID;
    prt->layer = layer;
    layers.emplace(std::make_pair(mCurID, layer));
    return FK_OK;
}

FkResult FkGraphicLayerQuark::_onUpdateLayer(std::shared_ptr<FkProtocol> p) {
    auto prt = Fk_POINTER_CAST(FkGraphicUpdateLayerPrt, p);
    auto itr = layers.find(prt->layer->id);
    if (layers.end() == itr) {
        return FK_FAIL;
    }

    std::vector<std::shared_ptr<FkGraphicComponent>> vec;
    if (FK_OK == prt->layer->findComponent(vec, FkClassType::type<FkColorComponent>())) {
        itr->second->addComponent(vec[0]);
    }
    vec.clear();
    if (FK_OK == prt->layer->findComponent(vec, FkClassType::type<FkSizeComponent>())) {
        itr->second->addComponent(vec[0]);
    }
    vec.clear();
    if (FK_OK == prt->layer->findComponent(vec, FkClassType::type<FkTexComponent>())) {
        itr->second->addComponent(vec[0]);
    }
    return FK_OK;
}

FkResult FkGraphicLayerQuark::_onRenderRequest(std::shared_ptr<FkProtocol> p) {
    auto prt = std::static_pointer_cast<FkRenderRequestPrt>(p);
    if (nullptr == prt->req) {
        prt->req = std::make_shared<FkRenderRequest>();
    }
    for (auto &it : layers) {
        prt->req->layers.emplace_back(it.second);
    }
    return FK_OK;
}