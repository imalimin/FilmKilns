/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicProgramQuark.h"
#include "FkGraphicLayerPrt.h"

FkGraphicProgramQuark::FkGraphicProgramQuark() : FkQuark() {

}

FkGraphicProgramQuark::~FkGraphicProgramQuark() {

}

void FkGraphicProgramQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicLayerPrt, FkGraphicProgramQuark::_onDrawLayer);
}

FkResult FkGraphicProgramQuark::onCreate() {
    return FkQuark::onCreate();
}

FkResult FkGraphicProgramQuark::onDestroy() {
    return FkQuark::onDestroy();
}

FkResult FkGraphicProgramQuark::onStart() {
    return FkQuark::onStart();
}

FkResult FkGraphicProgramQuark::onStop() {
    return FkQuark::onStop();
}

FkResult FkGraphicProgramQuark::_onDrawLayer(std::shared_ptr<FkProtocol> p) {
    return FK_OK;
}
