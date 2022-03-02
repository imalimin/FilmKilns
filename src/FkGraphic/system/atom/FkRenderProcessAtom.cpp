/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkRenderProcessAtom.h"
#include "FkRenderDefine.h"

FkRenderProcessAtom::FkRenderProcessAtom() : FkSimpleAtom() {
    FK_MARK_SUPER
}

FkRenderProcessAtom::~FkRenderProcessAtom() {

}

void FkRenderProcessAtom::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderProto, FkRenderProcessAtom::_onRender);
}

void FkRenderProcessAtom::onConnect(std::shared_ptr<FkConnectChain> chain) {
}

FkResult FkRenderProcessAtom::onCreate() {
    auto ret = FkSimpleAtom::onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkRenderProcessAtom::onDestroy() {
    auto ret = FkSimpleAtom::onDestroy();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkRenderProcessAtom::onStart() {
    auto ret = FkSimpleAtom::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkRenderProcessAtom::onStop() {
    auto ret = FkSimpleAtom::onStop();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkRenderProcessAtom::_onRender(std::shared_ptr<FkProtocol> p) {
    return FK_OK;
}