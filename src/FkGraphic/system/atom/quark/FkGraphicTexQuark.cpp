/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicTexQuark.h"
#include "FkGraphicLayerPrt.h"

FkGraphicTexQuark::FkGraphicTexQuark() : FkQuark() {

}

FkGraphicTexQuark::~FkGraphicTexQuark() {

}

void FkGraphicTexQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicLayerPrt, FkGraphicTexQuark::_onDrawLayer);
}

FkResult FkGraphicTexQuark::onCreate() {
    return FkQuark::onCreate();
}

FkResult FkGraphicTexQuark::onDestroy() {
    return FkQuark::onDestroy();
}

FkResult FkGraphicTexQuark::onStart() {
    return FkQuark::onStart();
}

FkResult FkGraphicTexQuark::onStop() {
    return FkQuark::onStop();
}

FkResult FkGraphicTexQuark::_onDrawLayer(std::shared_ptr<FkProtocol> p) {
    return FK_OK;
}
