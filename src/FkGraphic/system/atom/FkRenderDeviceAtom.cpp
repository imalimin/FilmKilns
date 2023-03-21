/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkRenderDeviceAtom.h"
#include "FkRenderDefine.h"
#include "FkBufDeviceQuark.h"
#include "FkTexDeviceQuark.h"
#include "FkScreenQuark.h"

FK_IMPL_CLASS_TYPE(FkRenderDeviceAtom, FkSimpleAtom)

FkRenderDeviceAtom::FkRenderDeviceAtom() : FkSimpleAtom() {

}

FkRenderDeviceAtom::~FkRenderDeviceAtom() {

}

void FkRenderDeviceAtom::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderProto, FkRenderDeviceAtom::_onRender);
}

void FkRenderDeviceAtom::onConnect(std::shared_ptr<FkConnectChain> chain) {
    chain->next<FkTexDeviceQuark>()
            ->next<FkBufDeviceQuark>()
            ->next<FkScreenQuark>();
}

FkResult FkRenderDeviceAtom::onCreate() {
    auto ret = FkSimpleAtom::onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkRenderDeviceAtom::onDestroy() {
    auto ret = FkSimpleAtom::onDestroy();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkRenderDeviceAtom::onStart() {
    auto ret = FkSimpleAtom::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkRenderDeviceAtom::onStop() {
    auto ret = FkSimpleAtom::onStop();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkRenderDeviceAtom::_onRender(std::shared_ptr<FkProtocol> p) {
    return dispatchNext(p);
}