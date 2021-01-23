/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicRotateQuark.h"
#include "FkGraphicLayerPrt.h"

FkGraphicRotateQuark::FkGraphicRotateQuark() : FkQuark() {

}

FkGraphicRotateQuark::~FkGraphicRotateQuark() {

}

void FkGraphicRotateQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicLayerPrt, FkGraphicRotateQuark::_onDrawLayer);
}

FkResult FkGraphicRotateQuark::onCreate() {
    return FkQuark::onCreate();
}

FkResult FkGraphicRotateQuark::onDestroy() {
    return FkQuark::onDestroy();
}

FkResult FkGraphicRotateQuark::onStart() {
    return FkQuark::onStart();
}

FkResult FkGraphicRotateQuark::onStop() {
    return FkQuark::onStop();
}

FkResult FkGraphicRotateQuark::_onDrawLayer(std::shared_ptr<FkProtocol> p) {
    return FK_OK;
}
