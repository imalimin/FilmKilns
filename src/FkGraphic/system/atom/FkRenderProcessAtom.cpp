/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkRenderProcessAtom.h"

FkRenderProcessAtom::FkRenderProcessAtom() : FkSimpleMolecule() {
    FK_MARK_SUPER
}

FkRenderProcessAtom::~FkRenderProcessAtom() {

}

void FkRenderProcessAtom::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
//    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicLayerPrt, FkGraphicMolecule::_onDrawLayer);
}

void FkRenderProcessAtom::onConnect(std::shared_ptr<FkConnectChain> chain) {
}

FkResult FkRenderProcessAtom::onCreate() {
    auto ret = FkSimpleMolecule::onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkRenderProcessAtom::onDestroy() {
    auto ret = FkSimpleMolecule::onDestroy();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkRenderProcessAtom::onStart() {
    auto ret = FkSimpleMolecule::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkRenderProcessAtom::onStop() {
    auto ret = FkSimpleMolecule::onStop();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}