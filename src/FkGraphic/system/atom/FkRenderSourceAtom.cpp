/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkRenderSourceAtom.h"
#include "FkRenderDefine.h"
#include "FkRenderTexQuark.h"
#include "FkRenderFboQuark.h"
#include "FkRenderVboQuark.h"
#include "FkRenderProgramQuark.h"

FK_IMPL_CLASS_TYPE(FkRenderSourceAtom, FkProtocol)

FkRenderSourceAtom::FkRenderSourceAtom() : FkSimpleAtom() {

}

FkRenderSourceAtom::~FkRenderSourceAtom() {

}

void FkRenderSourceAtom::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
}

void FkRenderSourceAtom::onConnect(std::shared_ptr<FkConnectChain> chain) {
    chain->next<FkRenderFboQuark>()
            ->next<FkRenderTexQuark>()
            ->next<FkRenderVboQuark>()
            ->next<FkRenderProgramQuark>();
}

FkResult FkRenderSourceAtom::onCreate() {
    auto ret = FkSimpleAtom::onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkRenderSourceAtom::onDestroy() {
    auto ret = FkSimpleAtom::onDestroy();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkRenderSourceAtom::onStart() {
    auto ret = FkSimpleAtom::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkRenderSourceAtom::onStop() {
    auto ret = FkSimpleAtom::onStop();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}