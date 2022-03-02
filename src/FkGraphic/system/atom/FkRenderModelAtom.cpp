/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkRenderModelAtom.h"
#include "FkIDQuark.h"
#include "FkRenderDefine.h"
#include "FkRenderMvpQuark.h"
#include "FkRenderInfoQuark.h"
#include "FkPositionQuark.h"
#include "FkSizeCompo.h"

FkRenderModelAtom::FkRenderModelAtom() : FkSimpleAtom() {
    FK_MARK_SUPER
}

FkRenderModelAtom::~FkRenderModelAtom() {

}

void FkRenderModelAtom::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderProto, FkRenderModelAtom::_onRender);
    FK_PORT_DELIVERY(desc, FkNewTexProto, FkRenderModelAtom);
    FK_PORT_DELIVERY(desc, FkGenIDProto, FkRenderModelAtom);
}

void FkRenderModelAtom::onConnect(std::shared_ptr<FkConnectChain> chain) {
    chain->next<FkIDQuark>()
            ->next<FkRenderInfoQuark>()
            ->next<FkPositionQuark>()
            ->next<FkRenderMvpQuark>();
}

FkResult FkRenderModelAtom::onCreate() {
    auto ret = FkSimpleAtom::onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkRenderModelAtom::onDestroy() {
    auto ret = FkSimpleAtom::onDestroy();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkRenderModelAtom::onStart() {
    auto ret = FkSimpleAtom::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkRenderModelAtom::onStop() {
    auto ret = FkSimpleAtom::onStop();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkRenderModelAtom::_onRender(std::shared_ptr<FkProtocol> &p) {
    return dispatchNext(p);
}