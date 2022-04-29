/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicModelAtom.h"
#include "FkWinModelQuark.h"
#include "FkGraphicCanvasQuark.h"
#include "FkGraphicLayerQuark.h"
#include "FkGraphicMVPQuark.h"
#include "FkGraphicNewLayerPrt.h"
#include "FkGraphicUpdateLayerPrt.h"
#include "FkRenderRequestPrt.h"
#include "FkQuerySizeProto.h"
#include "FkLayerPostTransProto.h"
#include "FkLayerPostScaleProto.h"
#include "FkLayerPostRotateProto.h"
#include "FkDrawPointProto.h"
#include "FkSetSurfacePrt.h"
#include "FkMeasureTransProto.h"
#include "FkMeasurePointProto.h"
#include "FkQueryLayersProto.h"
#include "FkLayerSetTransProto.h"
#include "FkLayerSetRotateProto.h"
#include "FkLayerSetScaleProto.h"
#include "FkCropProto.h"
#include "FkQueryWinSizeProto.h"
#include "FkRemoveLayerProto.h"
#include "FkReadPixelsProto.h"
#include "FkScaleTypeProto.h"

FkGraphicModelAtom::FkGraphicModelAtom() : FkSimpleAtom() {
    FK_MARK_SUPER
}

FkGraphicModelAtom::~FkGraphicModelAtom() {

}

void FkGraphicModelAtom::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DELIVERY(desc, FkGraphicNewLayerPrt, FkGraphicModelAtom);
    FK_PORT_DELIVERY(desc, FkGraphicUpdateLayerPrt, FkGraphicModelAtom);
    FK_PORT_DELIVERY(desc, FkRenderRequestPrt, FkGraphicModelAtom);
    FK_PORT_DELIVERY(desc, FkQuerySizeProto, FkGraphicModelAtom);
    FK_PORT_DELIVERY(desc, FkLayerPostTransProto, FkGraphicModelAtom);
    FK_PORT_DELIVERY(desc, FkLayerPostScaleProto, FkGraphicModelAtom);
    FK_PORT_DELIVERY(desc, FkLayerPostRotateProto, FkGraphicModelAtom);
    FK_PORT_DELIVERY(desc, FkMeasureTransProto, FkGraphicModelAtom);
    FK_PORT_DELIVERY(desc, FkMeasurePointProto, FkGraphicModelAtom);
    FK_PORT_DELIVERY(desc, FkDrawPointProto, FkGraphicModelAtom);
    FK_PORT_DELIVERY(desc, FkSetSurfacePrt, FkGraphicModelAtom);
    FK_PORT_DELIVERY(desc, FkQueryLayersProto, FkGraphicModelAtom);
    FK_PORT_DELIVERY(desc, FkLayerSetTransProto, FkGraphicModelAtom);
    FK_PORT_DELIVERY(desc, FkLayerSetRotateProto, FkGraphicModelAtom);
    FK_PORT_DELIVERY(desc, FkLayerSetScaleProto, FkGraphicModelAtom);
    FK_PORT_DELIVERY(desc, FkCropProto, FkGraphicModelAtom);
    FK_PORT_DELIVERY(desc, FkQueryWinSizeProto, FkGraphicModelAtom);
    FK_PORT_DELIVERY(desc, FkRemoveLayerProto, FkGraphicModelAtom);
    FK_PORT_DELIVERY(desc, FkReadPixelsProto, FkGraphicModelAtom);
    FK_PORT_DELIVERY(desc, FkScaleTypeProto, FkGraphicModelAtom);
}

void FkGraphicModelAtom::onConnect(std::shared_ptr<FkConnectChain> chain) {
    chain->next<FkWinModelQuark>()
            ->next<FkGraphicCanvasQuark>()
            ->next<FkGraphicLayerQuark>()
            ->next<FkGraphicMVPQuark>();
}

FkResult FkGraphicModelAtom::onCreate() {
    auto ret = FkSimpleAtom::onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkGraphicModelAtom::onDestroy() {
    auto ret = FkSimpleAtom::onDestroy();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkGraphicModelAtom::onStart() {
    auto ret = FkSimpleAtom::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkGraphicModelAtom::onStop() {
    auto ret = FkSimpleAtom::onStop();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkGraphicModelAtom::_onDrawLayer(std::shared_ptr<FkProtocol> p) {
    return dispatchNext(p);
}