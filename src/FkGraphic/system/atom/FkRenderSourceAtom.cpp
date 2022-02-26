/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkRenderSourceAtom.h"
#include "FkRenderDefine.h"
#include "FkNewTexProto.h"
#include "FkRenderTexQuark.h"
#include "FkRenderFboQuark.h"

FkRenderSourceAtom::FkRenderSourceAtom() : FkSimpleMolecule() {
    FK_MARK_SUPER
}

FkRenderSourceAtom::~FkRenderSourceAtom() {

}

void FkRenderSourceAtom::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DELIVERY(desc, FkRenderProto, FkRenderSourceAtom);
    FK_PORT_DELIVERY(desc, FkNewTexProto, FkRenderSourceAtom);
}

void FkRenderSourceAtom::onConnect(std::shared_ptr<FkConnectChain> chain) {
    chain->next<FkRenderFboQuark>()
            ->next<FkRenderTexQuark>();
}

FkResult FkRenderSourceAtom::onCreate() {
    auto ret = FkSimpleMolecule::onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkRenderSourceAtom::onDestroy() {
    auto ret = FkSimpleMolecule::onDestroy();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkRenderSourceAtom::onStart() {
    auto ret = FkSimpleMolecule::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkRenderSourceAtom::onStop() {
    auto ret = FkSimpleMolecule::onStop();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkRenderSourceAtom::_onRender(std::shared_ptr<FkProtocol> p) {
    return FK_OK;
}