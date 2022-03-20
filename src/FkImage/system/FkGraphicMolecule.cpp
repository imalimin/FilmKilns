/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicMolecule.h"
#include "FkGraphicModelAtom.h"
#include "FkGraphicSourceAtom.h"
#include "FkGraphicRenderAtom.h"
#include "FkGraphicNewLayerPrt.h"
#include "FkGraphicUpdateLayerPrt.h"
#include "FkSetSurfacePrt.h"
#include "FkRenderRequestPrt.h"
#include "FkQuerySizeProto.h"
#include "FkLayerPostTransProto.h"
#include "FkLayerPostScaleProto.h"
#include "FkLayerPostRotateProto.h"
#include "FkMeasureTransProto.h"
#include "FkMeasurePointProto.h"
#include "FkDrawPointProto.h"
#include "FkQueryLayersProto.h"
#include "FkRenderEngineCompo.h"
#include "FkLayerSetTransProto.h"
#include "FkLayerSetRotateProto.h"
#include "FkLayerSetScaleProto.h"

FkGraphicMolecule::FkGraphicMolecule() : FkSimpleMolecule() {
    FK_MARK_SUPER
}

FkGraphicMolecule::~FkGraphicMolecule() {

}

void FkGraphicMolecule::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DELIVERY(desc, FkGraphicNewLayerPrt, FkGraphicMolecule);
    FK_PORT_DELIVERY(desc, FkGraphicUpdateLayerPrt, FkGraphicMolecule);
    FK_PORT_DELIVERY(desc, FkSetSurfacePrt, FkGraphicMolecule);
    FK_PORT_DELIVERY(desc, FkRenderRequestPrt, FkGraphicMolecule);
    FK_PORT_DELIVERY(desc, FkQuerySizeProto, FkGraphicMolecule);
    FK_PORT_DELIVERY(desc, FkLayerPostTransProto, FkGraphicMolecule);
    FK_PORT_DELIVERY(desc, FkLayerPostScaleProto, FkGraphicMolecule);
    FK_PORT_DELIVERY(desc, FkLayerPostRotateProto, FkGraphicMolecule);
    FK_PORT_DELIVERY(desc, FkMeasureTransProto, FkGraphicMolecule);
    FK_PORT_DELIVERY(desc, FkMeasurePointProto, FkGraphicMolecule);
    FK_PORT_DELIVERY(desc, FkDrawPointProto, FkGraphicMolecule);
    FK_PORT_DELIVERY(desc, FkQueryLayersProto, FkGraphicMolecule);
    FK_PORT_DELIVERY(desc, FkLayerSetTransProto, FkGraphicMolecule);
    FK_PORT_DELIVERY(desc, FkLayerSetRotateProto, FkGraphicMolecule);
    FK_PORT_DELIVERY(desc, FkLayerSetScaleProto, FkGraphicMolecule);
}

void FkGraphicMolecule::onConnect(std::shared_ptr<FkConnectChain> chain) {
    chain->next<FkGraphicModelAtom>()
            ->next<FkGraphicSourceAtom>()
            ->next<FkGraphicRenderAtom>();
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
    return dispatchNext(p);
}
