/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicMolecule.h"
#include "FkGraphicLayerPrt.h"
#include "FkGraphicModelAtom.h"
#include "FkGraphicSourceAtom.h"
#include "FkGraphicRenderAtom.h"
#include "FkGraphicScreenAtom.h"
#include "FkGraphicNewLayerPrt.h"
#include "FkGraphicUpdateLayerPrt.h"
#include "FkGraphicNewTexPtl.h"
#include "FkSetSurfacePrt.h"
#include "FkRenderRequestPrt.h"
#include "FkGraphicUpdateTexPrt.h"
#include "FkGraphicTexDelPtl.h"
#include "FkUpdateTexWithBmpPrt.h"
#include "FkSetSizeProto.h"
#include "FkQuerySizeProto.h"
#include "FkGraphicUpdateCanvasProto.h"
#include "FkLayerPostTransProto.h"
#include "FkLayerPostScaleProto.h"
#include "FkLayerPostRotateProto.h"
#include "FkMeasureTransProto.h"

FkGraphicMolecule::FkGraphicMolecule() : FkSimpleMolecule() {
    FK_MARK_SUPER
}

FkGraphicMolecule::~FkGraphicMolecule() {

}

void FkGraphicMolecule::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicLayerPrt, FkGraphicMolecule::_onDrawLayer);
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicNewLayerPrt, FkGraphicMolecule::dispatchNext);
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicUpdateLayerPrt, FkGraphicMolecule::dispatchNext);
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicUpdateTexPrt, FkGraphicMolecule::dispatchNext);
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicNewTexPtl, FkGraphicMolecule::dispatchNext);
    FK_PORT_DESC_QUICK_ADD(desc, FkSetSurfacePrt, FkGraphicMolecule::dispatchNext);
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderRequestPrt, FkGraphicMolecule::dispatchNext);
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicTexDelPtl, FkGraphicMolecule::dispatchNext);
    FK_PORT_DESC_QUICK_ADD(desc, FkUpdateTexWithBmpPrt, FkGraphicMolecule::dispatchNext);
    FK_PORT_DESC_QUICK_ADD(desc, FkSetSizeProto, FkGraphicMolecule::dispatchNext);
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicUpdateCanvasProto, FkGraphicMolecule::dispatchNext);
    FK_PORT_DESC_QUICK_ADD(desc, FkQuerySizeProto, FkGraphicMolecule::dispatchNext);
    FK_PORT_DESC_QUICK_ADD(desc, FkLayerPostTransProto, FkGraphicMolecule::dispatchNext);
    FK_PORT_DESC_QUICK_ADD(desc, FkLayerPostScaleProto, FkGraphicMolecule::dispatchNext);
    FK_PORT_DESC_QUICK_ADD(desc, FkLayerPostRotateProto, FkGraphicMolecule::dispatchNext);
    FK_PORT_DESC_QUICK_ADD(desc, FkMeasureTransProto, FkGraphicMolecule::dispatchNext);
}

void FkGraphicMolecule::onConnect(std::shared_ptr<FkConnectChain> chain) {
    chain->next<FkGraphicModelAtom>()
            ->next<FkGraphicSourceAtom>()
            ->next<FkGraphicRenderAtom>()
            ->next<FkGraphicScreenAtom>();
}

FkResult FkGraphicMolecule::onCreate() {
    auto ret = FkSimpleMolecule::onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkGraphicMolecule::onDestroy() {
    auto ret = FkSimpleMolecule::onDestroy();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkGraphicMolecule::onStart() {
    auto ret = FkSimpleMolecule::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkGraphicMolecule::onStop() {
    auto ret = FkSimpleMolecule::onStop();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkGraphicMolecule::_onDrawLayer(std::shared_ptr<FkProtocol> p) {
    return dispatchNext(std::move(p));
}
