/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkRenderProcessAtom.h"
#include "FkRenderDefine.h"
#include "FkPointProcessQuark.h"
#include "FkRenderPathQuark.h"

FK_IMPL_CLASS_TYPE(FkRenderProcessAtom, FkSimpleAtom)

FkRenderProcessAtom::FkRenderProcessAtom() : FkSimpleAtom() {

}

FkRenderProcessAtom::~FkRenderProcessAtom() {

}

void FkRenderProcessAtom::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
}

void FkRenderProcessAtom::onConnect(std::shared_ptr<FkConnectChain> chain) {
    chain->next<FkPointProcessQuark>()
            ->next<FkRenderPathQuark>();
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