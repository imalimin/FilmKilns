/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicMolecule.h"
#include "FkGraphicLayerPrt.h"
#include "FkGraphicMVPAtom.h"
#include "FkGraphicSourceAtom.h"
#include "FkGraphicRenderAtom.h"
#include "FkGraphicScreenAtom.h"
#include "FkGraphicNewLayerPrt.h"

FkGraphicMolecule::FkGraphicMolecule() : FkSimpleMolecule() {
    FK_MARK_SUPER
}

FkGraphicMolecule::~FkGraphicMolecule() {

}

void FkGraphicMolecule::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicLayerPrt, FkGraphicMolecule::_onDrawLayer);
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicNewLayerPrt, FkGraphicMolecule::_onNewLayer);
}

void FkGraphicMolecule::onConnect(std::shared_ptr<FkConnectChain> chain) {
    chain->next<FkGraphicMVPAtom>();
    chain->next<FkGraphicSourceAtom>();
    chain->next<FkGraphicRenderAtom>();
    chain->next<FkGraphicScreenAtom>();
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

FkResult FkGraphicMolecule::_onNewLayer(std::shared_ptr<FkProtocol> p) {
    return dispatchNext(std::move(p));
}
