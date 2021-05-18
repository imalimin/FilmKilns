/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicMVPAtom.h"
#include "FkGraphicLayerPrt.h"
#include "FkGraphicTransQuark.h"
#include "FkGraphicScaleQuark.h"
#include "FkGraphicRotateQuark.h"
#include "FkGraphicNewLayerPrt.h"
#include "FkGraphicUpdateLayerPrt.h"
#include "FkGraphicLayerQuark.h"
#include "FkGraphicNewTexPtl.h"

FkGraphicMVPAtom::FkGraphicMVPAtom() : FkSimpleAtom() {
    FK_MARK_SUPER
}

FkGraphicMVPAtom::~FkGraphicMVPAtom() {

}

void FkGraphicMVPAtom::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicLayerPrt, FkGraphicMVPAtom::_onDrawLayer);
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicNewLayerPrt, FkGraphicMVPAtom::dispatchNext);
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicUpdateLayerPrt, FkGraphicMVPAtom::dispatchNext);
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicNewTexPtl, FkGraphicMVPAtom::dispatchNext);
}

void FkGraphicMVPAtom::onConnect(std::shared_ptr<FkConnectChain> chain) {
    chain->next<FkGraphicLayerQuark>();
    chain->next<FkGraphicTransQuark>();
    chain->next<FkGraphicScaleQuark>();
    chain->next<FkGraphicRotateQuark>();
}

FkResult FkGraphicMVPAtom::onCreate() {
    auto ret = FkSimpleAtom::onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkGraphicMVPAtom::onDestroy() {
    auto ret = FkSimpleAtom::onDestroy();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkGraphicMVPAtom::onStart() {
    auto ret = FkSimpleAtom::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkGraphicMVPAtom::onStop() {
    auto ret = FkSimpleAtom::onStop();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkGraphicMVPAtom::_onDrawLayer(std::shared_ptr<FkProtocol> p) {
    return dispatchNext(std::move(p));
}