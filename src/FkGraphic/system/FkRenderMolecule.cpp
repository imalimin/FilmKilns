/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkRenderMolecule.h"
#include "FkRenderModelAtom.h"
#include "FkRenderSourceAtom.h"
#include "FkRenderProcessAtom.h"
#include "FkRenderDeviceAtom.h"

FkRenderMolecule::FkRenderMolecule() : FkSimpleMolecule() {
    FK_MARK_SUPER
}

FkRenderMolecule::~FkRenderMolecule() {

}

void FkRenderMolecule::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
//    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicLayerPrt, FkGraphicMolecule::_onDrawLayer);
}

void FkRenderMolecule::onConnect(std::shared_ptr<FkConnectChain> chain) {
    chain->next<FkRenderModelAtom>()
            ->next<FkRenderSourceAtom>()
            ->next<FkRenderProcessAtom>()
            ->next<FkRenderDeviceAtom>();
}

FkResult FkRenderMolecule::onCreate() {
    auto ret = FkSimpleMolecule::onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkRenderMolecule::onDestroy() {
    auto ret = FkSimpleMolecule::onDestroy();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkRenderMolecule::onStart() {
    auto ret = FkSimpleMolecule::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkRenderMolecule::onStop() {
    auto ret = FkSimpleMolecule::onStop();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}