/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicSourceAtom.h"
#include "FkGraphicLayerPrt.h"
#include "FkGraphicContextQuark.h"
#include "FkGraphicTexQuark.h"
#include "FkGraphicFBOQuark.h"
#include "FkGraphicProgramQuark.h"
#include "FkGraphicNewTexPtl.h"

FkGraphicSourceAtom::FkGraphicSourceAtom() : FkSimpleAtom() {
    FK_MARK_SUPER
}

FkGraphicSourceAtom::~FkGraphicSourceAtom() {

}

void FkGraphicSourceAtom::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicLayerPrt, FkGraphicSourceAtom::_onDrawLayer);
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicNewTexPtl, FkGraphicSourceAtom::dispatchNext);
}

void FkGraphicSourceAtom::onConnect(std::shared_ptr<FkConnectChain> chain) {
    chain->next<FkGraphicContextQuark>();
    chain->next<FkGraphicTexQuark>();
    chain->next<FkGraphicFBOQuark>();
    chain->next<FkGraphicProgramQuark>();
}

FkResult FkGraphicSourceAtom::onCreate() {
    auto ret = FkSimpleAtom::onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkGraphicSourceAtom::onDestroy() {
    auto ret = FkSimpleAtom::onDestroy();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkGraphicSourceAtom::onStart() {
    auto ret = FkSimpleAtom::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkGraphicSourceAtom::onStop() {
    auto ret = FkSimpleAtom::onStop();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkGraphicSourceAtom::_onDrawLayer(std::shared_ptr<FkProtocol> p) {
    return dispatchNext(std::move(p));
}

FkResult FkGraphicSourceAtom::dispatchNext(std::shared_ptr<FkProtocol> p) {
    auto ret = FkSimpleAtom::dispatchNext(p);
    return ret;
}
