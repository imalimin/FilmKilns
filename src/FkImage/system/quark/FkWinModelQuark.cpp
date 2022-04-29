/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkWinModelQuark.h"
#include "FkRenderRequestPrt.h"
#include "FkMeasureTransProto.h"
#include "FkMeasurePointProto.h"
#include "FkGraphicUpdateLayerPrt.h"
#include "FkSetSurfacePrt.h"
#include "FkQueryWinSizeProto.h"
#include "FkScaleTypeProto.h"

FkWinModelQuark::FkWinModelQuark() : FkQuark(), winSize(0, 0) {
    FK_MARK_SUPER
}

FkWinModelQuark::~FkWinModelQuark() {

}

void FkWinModelQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkSetSurfacePrt, FkWinModelQuark::_onSetViewSize);
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderRequestPrt, FkWinModelQuark::_onWithWinSize);
    FK_PORT_DESC_QUICK_ADD(desc, FkMeasureTransProto, FkWinModelQuark::_onWithWinSize);
    FK_PORT_DESC_QUICK_ADD(desc, FkMeasurePointProto, FkWinModelQuark::_onWithWinSize);
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicUpdateLayerPrt, FkWinModelQuark::_onWithWinSize);
    FK_PORT_DESC_QUICK_ADD(desc, FkQueryWinSizeProto, FkWinModelQuark::_onWithWinSize);
    FK_PORT_DESC_QUICK_ADD(desc, FkScaleTypeProto, FkWinModelQuark::_onWithWinSize);
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
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkSetSurfacePrt, p);
    winSize = proto->win->size();
    return FK_OK;
}

FkResult FkWinModelQuark::_onWithWinSize(std::shared_ptr<FkProtocol> p) {
    if (FK_INSTANCE_OF(p, FkWinSizeProto)) {
        FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkWinSizeProto, p);
        proto->winSize = winSize;
    }
    return FK_OK;
}