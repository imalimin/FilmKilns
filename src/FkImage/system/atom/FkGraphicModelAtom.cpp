/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicModelAtom.h"
#include "FkWinModelQuark.h"
#include "FkGraphicLayerPrt.h"
#include "FkGraphicMVPQuark.h"
#include "FkGraphicCanvasQuark.h"
#include "FkGraphicNewLayerPrt.h"
#include "FkGraphicUpdateLayerPrt.h"
#include "FkGraphicLayerQuark.h"
#include "FkGraphicNewTexPtl.h"
#include "FkRenderRequestPrt.h"
#include "FkSetSizeProto.h"
#include "FkQuerySizeProto.h"
#include "FkLayerPostTransProto.h"
#include "FkLayerPostScaleProto.h"
#include "FkLayerPostRotateProto.h"
#include "FkMeasureTransProto.h"
#include "FkDrawPointProto.h"

FkGraphicModelAtom::FkGraphicModelAtom() : FkSimpleAtom() {
    FK_MARK_SUPER
}

FkGraphicModelAtom::~FkGraphicModelAtom() {

}

void FkGraphicModelAtom::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicLayerPrt, FkGraphicModelAtom::_onDrawLayer);
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicNewLayerPrt, FkGraphicModelAtom::dispatchNext);
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicUpdateLayerPrt, FkGraphicModelAtom::dispatchNext);
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicNewTexPtl, FkGraphicModelAtom::dispatchNext);
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderRequestPrt, FkGraphicModelAtom::dispatchNext);
    FK_PORT_DESC_QUICK_ADD(desc, FkSetSizeProto, FkGraphicModelAtom::dispatchNext);
    FK_PORT_DESC_QUICK_ADD(desc, FkQuerySizeProto, FkGraphicModelAtom::dispatchNext);
    FK_PORT_DESC_QUICK_ADD(desc, FkLayerPostTransProto, FkGraphicModelAtom::dispatchNext);
    FK_PORT_DESC_QUICK_ADD(desc, FkLayerPostScaleProto, FkGraphicModelAtom::dispatchNext);
    FK_PORT_DESC_QUICK_ADD(desc, FkLayerPostRotateProto, FkGraphicModelAtom::dispatchNext);
    FK_PORT_DESC_QUICK_ADD(desc, FkMeasureTransProto, FkGraphicModelAtom::dispatchNext);
    FK_PORT_DESC_QUICK_ADD(desc, FkDrawPointProto, FkGraphicModelAtom::dispatchNext);
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
    return dispatchNext(std::move(p));
}

FkResult FkGraphicModelAtom::dispatchNext(std::shared_ptr<FkProtocol> p) {
    auto ret = FkSimpleAtom::dispatchNext(p);
    return ret;
}