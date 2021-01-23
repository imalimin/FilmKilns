/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicScaleQuark.h"
#include "FkGraphicLayerPrt.h"

FkGraphicScaleQuark::FkGraphicScaleQuark() : FkQuark() {

}

FkGraphicScaleQuark::~FkGraphicScaleQuark() {

}

void FkGraphicScaleQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicLayerPrt, FkGraphicScaleQuark::_onDrawLayer);
}

FkResult FkGraphicScaleQuark::onCreate() {
    return FkQuark::onCreate();
}

FkResult FkGraphicScaleQuark::onDestroy() {
    return FkQuark::onDestroy();
}

FkResult FkGraphicScaleQuark::onStart() {
    return FkQuark::onStart();
}

FkResult FkGraphicScaleQuark::onStop() {
    return FkQuark::onStop();
}

FkResult FkGraphicScaleQuark::_onDrawLayer(std::shared_ptr<FkProtocol> p) {
    return FK_OK;
}