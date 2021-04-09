/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicMVPAtom.h"
#include "FkGraphicLayerPrt.h"
#include "FkGraphicTransQuark.h"
#include "FkGraphicScaleQuark.h"
#include "FkGraphicRotateQuark.h"

FkGraphicMVPAtom::FkGraphicMVPAtom() : FkSimpleAtom() {
}

FkGraphicMVPAtom::~FkGraphicMVPAtom() {

}

void FkGraphicMVPAtom::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicLayerPrt, FkGraphicMVPAtom::_onDrawLayer);
}

void FkGraphicMVPAtom::onConnect(std::shared_ptr<FkConnectChain> chain) {
    chain->next(std::make_shared<FkGraphicTransQuark>());
    chain->next(std::make_shared<FkGraphicScaleQuark>());
    chain->next(std::make_shared<FkGraphicRotateQuark>());
}

FkResult FkGraphicMVPAtom::onCreate() {
    auto ret = FkSimpleAtom::onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkGraphicMVPAtom::onDestroy() {
    auto ret = FkSimpleAtom::onDestroy();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkGraphicMVPAtom::onStart() {
    auto ret = FkSimpleAtom::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkGraphicMVPAtom::onStop() {
    auto ret = FkSimpleAtom::onStop();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkGraphicMVPAtom::_onDrawLayer(std::shared_ptr<FkProtocol> p) {
    return dispatchNext(std::move(p));
}