/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicTransQuark.h"
#include "FkGraphicLayerPrt.h"

FkGraphicTransQuark::FkGraphicTransQuark() : FkQuark() {
    FK_MARK_SUPER

}

FkGraphicTransQuark::~FkGraphicTransQuark() {

}

void FkGraphicTransQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicLayerPrt, FkGraphicTransQuark::_onDrawLayer);
}

FkResult FkGraphicTransQuark::onCreate() {
    return FkQuark::onCreate();
}

FkResult FkGraphicTransQuark::onDestroy() {
    return FkQuark::onDestroy();
}

FkResult FkGraphicTransQuark::onStart() {
    return FkQuark::onStart();
}

FkResult FkGraphicTransQuark::onStop() {
    return FkQuark::onStop();
}

FkResult FkGraphicTransQuark::_onDrawLayer(std::shared_ptr<FkProtocol> p) {
    return FK_OK;
}
