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
#include "FkLayerPostTransProto.h"
#include "FkLayerPostScaleProto.h"
#include "FkLayerPostRotateProto.h"
#include "FkMeasureTransProto.h"
#include "FkDrawPointProto.h"
#include "FkRenderEngineCompo.h"

#define RENDER_ALIAS "RenderEngine"

FkGraphicMolecule::FkGraphicMolecule() : FkSimpleMolecule(),
                                         renderEngine(
                                                 std::make_shared<FkRenderEngine>(RENDER_ALIAS)) {
    FK_MARK_SUPER
}

FkGraphicMolecule::~FkGraphicMolecule() {

}

void FkGraphicMolecule::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicLayerPrt, FkGraphicMolecule::_onDrawLayer);
    FK_PORT_DELIVERY(desc, FkGraphicNewLayerPrt, FkGraphicMolecule);
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicUpdateLayerPrt, FkGraphicMolecule::dispatchNext);
    FK_PORT_DELIVERY(desc, FkGraphicUpdateTexPrt, FkGraphicMolecule);
    FK_PORT_DELIVERY(desc, FkGraphicNewTexPtl, FkGraphicMolecule);
    FK_PORT_DELIVERY(desc, FkSetSurfacePrt, FkGraphicMolecule);
    FK_PORT_DELIVERY(desc, FkRenderRequestPrt, FkGraphicMolecule);
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicTexDelPtl, FkGraphicMolecule::dispatchNext);
    FK_PORT_DESC_QUICK_ADD(desc, FkUpdateTexWithBmpPrt, FkGraphicMolecule::dispatchNext);
    FK_PORT_DELIVERY(desc, FkSetSizeProto, FkGraphicMolecule);
    FK_PORT_DELIVERY(desc, FkQuerySizeProto, FkGraphicMolecule);
    FK_PORT_DESC_QUICK_ADD(desc, FkLayerPostTransProto, FkGraphicMolecule::dispatchNext);
    FK_PORT_DESC_QUICK_ADD(desc, FkLayerPostScaleProto, FkGraphicMolecule::dispatchNext);
    FK_PORT_DESC_QUICK_ADD(desc, FkLayerPostRotateProto, FkGraphicMolecule::dispatchNext);
    FK_PORT_DESC_QUICK_ADD(desc, FkMeasureTransProto, FkGraphicMolecule::dispatchNext);
    FK_PORT_DESC_QUICK_ADD(desc, FkDrawPointProto, FkGraphicMolecule::dispatchNext);
}

std::shared_ptr<FkQuarkContext> FkGraphicMolecule::shareContextToSubQuark() {
    if (context == nullptr) {
        context = std::make_shared<FkQuarkContext>();
        context->addComponent(std::make_shared<FkRenderEngineCompo>(renderEngine));
    }
    return context;
}

void FkGraphicMolecule::onConnect(std::shared_ptr<FkConnectChain> chain) {
    chain->next<FkGraphicModelAtom>()
            ->next<FkGraphicSourceAtom>()
            ->next<FkGraphicRenderAtom>();
//            ->next<FkGraphicScreenAtom>();
}

FkResult FkGraphicMolecule::onCreate() {
    renderEngine->create();
    auto ret = FkSimpleMolecule::onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkGraphicMolecule::onDestroy() {
    auto ret = FkSimpleMolecule::onDestroy();
    renderEngine->destroy();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkGraphicMolecule::onStart() {
    renderEngine->start();
    auto ret = FkSimpleMolecule::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkGraphicMolecule::onStop() {
    auto ret = FkSimpleMolecule::onStop();
    renderEngine->stop();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkGraphicMolecule::_onDrawLayer(std::shared_ptr<FkProtocol> p) {
    return dispatchNext(p);
}
