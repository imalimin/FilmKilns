/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkRenderDeviceAtom.h"
#include "FkRenderDefine.h"
#include "FkBufDeviceQuark.h"

FkRenderDeviceAtom::FkRenderDeviceAtom() : FkSimpleMolecule() {
    FK_MARK_SUPER
}

FkRenderDeviceAtom::~FkRenderDeviceAtom() {

}

void FkRenderDeviceAtom::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderProto, FkRenderDeviceAtom::_onRender);
}

void FkRenderDeviceAtom::onConnect(std::shared_ptr<FkConnectChain> chain) {
    chain->next<FkBufDeviceQuark>();
}

FkResult FkRenderDeviceAtom::onCreate() {
    auto ret = FkSimpleMolecule::onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkRenderDeviceAtom::onDestroy() {
    auto ret = FkSimpleMolecule::onDestroy();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkRenderDeviceAtom::onStart() {
    auto ret = FkSimpleMolecule::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkRenderDeviceAtom::onStop() {
    auto ret = FkSimpleMolecule::onStop();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkRenderDeviceAtom::_onRender(std::shared_ptr<FkProtocol> p) {
    return dispatchNext(p);
}