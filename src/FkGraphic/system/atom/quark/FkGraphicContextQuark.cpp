/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicContextQuark.h"
#include "FkGraphicLayerPrt.h"

FkGraphicContextQuark::FkGraphicContextQuark() : FkQuark() {

}

FkGraphicContextQuark::~FkGraphicContextQuark() {

}

void FkGraphicContextQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicLayerPrt, FkGraphicContextQuark::_onDrawLayer);
}

FkResult FkGraphicContextQuark::onCreate() {
    return FkQuark::onCreate();
}

FkResult FkGraphicContextQuark::onDestroy() {
    return FkQuark::onDestroy();
}

FkResult FkGraphicContextQuark::onStart() {
    return FkQuark::onStart();
}

FkResult FkGraphicContextQuark::onStop() {
    return FkQuark::onStop();
}

FkResult FkGraphicContextQuark::_onDrawLayer(std::shared_ptr<FkProtocol> p) {
    return FK_OK;
}
