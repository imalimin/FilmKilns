/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicModelAtom.h"
#include "FkWinModelQuark.h"
#include "FkGraphicCanvasQuark.h"
#include "FkGraphicLayerQuark.h"
#include "FkGraphicMVPQuark.h"

FK_IMPL_CLASS_TYPE(FkGraphicModelAtom, FkSimpleAtom)

FkGraphicModelAtom::FkGraphicModelAtom() : FkSimpleAtom() {

}

FkGraphicModelAtom::~FkGraphicModelAtom() {

}

void FkGraphicModelAtom::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
}

void FkGraphicModelAtom::onConnect(std::shared_ptr<FkConnectChain> chain) {
    chain->next<FkWinModelQuark>()
            ->next<FkGraphicCanvasQuark>()
            ->next<FkGraphicLayerQuark>()
            ->next<FkGraphicMVPQuark>();
}

FkResult FkGraphicModelAtom::onCreate() {
    auto ret = FkSimpleAtom::onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkGraphicModelAtom::onDestroy() {
    auto ret = FkSimpleAtom::onDestroy();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkGraphicModelAtom::onStart() {
    auto ret = FkSimpleAtom::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkGraphicModelAtom::onStop() {
    auto ret = FkSimpleAtom::onStop();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkGraphicModelAtom::_onDrawLayer(std::shared_ptr<FkProtocol> p) {
    return dispatchNext(p);
}