/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkWinModelQuark.h"
#include "FkSetSizeProto.h"
#include "FkRenderRequestPrt.h"
#include "FkMeasureTransProto.h"
#include "FkGraphicUpdateLayerPrt.h"

FkWinModelQuark::FkWinModelQuark() : FkQuark(), winSize(0, 0) {
    FK_MARK_SUPER
}

FkWinModelQuark::~FkWinModelQuark() {

}

void FkWinModelQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkSetSizeProto, FkWinModelQuark::_onSetViewSize);
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderRequestPrt, FkWinModelQuark::_onWithWinSize);
    FK_PORT_DESC_QUICK_ADD(desc, FkMeasureTransProto, FkWinModelQuark::_onWithWinSize);
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicUpdateLayerPrt, FkWinModelQuark::_onWithWinSize);
}

FkResult FkWinModelQuark::onCreate() {
    return FkQuark::onCreate();
}

FkResult FkWinModelQuark::onDestroy() {
    return FkQuark::onDestroy();
}

FkResult FkWinModelQuark::onStart() {
    return FkQuark::onStart();
}

FkResult FkWinModelQuark::onStop() {
    return FkQuark::onStop();
}

FkResult FkWinModelQuark::_onSetViewSize(std::shared_ptr<FkProtocol> p) {
    auto prt = Fk_POINTER_CAST(FkSetSizeProto, p);
    winSize = prt->value;
    return FK_OK;
}

FkResult FkWinModelQuark::_onWithWinSize(std::shared_ptr<FkProtocol> p) {
    if (FK_INSTANCE_OF(p, FkWinSizeProto)) {
        auto proto = Fk_POINTER_CAST(FkWinSizeProto, p);
        proto->winSize = winSize;
    }
    return FK_OK;
}