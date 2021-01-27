/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicFBOQuark.h"
#include "FkGraphicLayerPrt.h"

FkGraphicFBOQuark::FkGraphicFBOQuark() : FkQuark() {

}

FkGraphicFBOQuark::~FkGraphicFBOQuark() {

}

void FkGraphicFBOQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicLayerPrt, FkGraphicFBOQuark::_onDrawLayer);
}

FkResult FkGraphicFBOQuark::onCreate() {
    return FkQuark::onCreate();
}

FkResult FkGraphicFBOQuark::onDestroy() {
    return FkQuark::onDestroy();
}

FkResult FkGraphicFBOQuark::onStart() {
    return FkQuark::onStart();
}

FkResult FkGraphicFBOQuark::onStop() {
    return FkQuark::onStop();
}

FkResult FkGraphicFBOQuark::_onDrawLayer(std::shared_ptr<FkProtocol> p) {
    return FK_OK;
}