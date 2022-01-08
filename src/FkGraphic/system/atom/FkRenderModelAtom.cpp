/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkRenderModelAtom.h"

FkRenderModelAtom::FkRenderModelAtom() : FkSimpleMolecule() {
    FK_MARK_SUPER
}

FkRenderModelAtom::~FkRenderModelAtom() {

}

void FkRenderModelAtom::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
//    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicLayerPrt, FkGraphicMolecule::_onDrawLayer);
}

void FkRenderModelAtom::onConnect(std::shared_ptr<FkConnectChain> chain) {
}

FkResult FkRenderModelAtom::onCreate() {
    auto ret = FkSimpleMolecule::onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkRenderModelAtom::onDestroy() {
    auto ret = FkSimpleMolecule::onDestroy();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkRenderModelAtom::onStart() {
    auto ret = FkSimpleMolecule::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkRenderModelAtom::onStop() {
    auto ret = FkSimpleMolecule::onStop();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}