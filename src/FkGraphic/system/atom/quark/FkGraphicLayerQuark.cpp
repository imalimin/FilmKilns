/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicLayerQuark.h"
#include "FkGraphicNewLayerPrt.h"

FkGraphicLayerQuark::FkGraphicLayerQuark() : FkQuark() {
    FK_MARK_SUPER
}

FkGraphicLayerQuark::~FkGraphicLayerQuark() {

}

void FkGraphicLayerQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicNewLayerPrt, FkGraphicLayerQuark::_onNewLayer);
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
    ids.emplace_back(mCurID);
    return FK_OK;
}