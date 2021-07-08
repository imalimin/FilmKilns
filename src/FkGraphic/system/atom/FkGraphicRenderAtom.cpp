/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicRenderAtom.h"
#include "FkRenderRequestPrt.h"
#include "FkPreRenderQuark.h"
#include "FkLayerRenderQuark.h"

FkGraphicRenderAtom::FkGraphicRenderAtom() : FkSimpleAtom() {
    FK_MARK_SUPER
}

FkGraphicRenderAtom::~FkGraphicRenderAtom() {

}

void FkGraphicRenderAtom::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderRequestPrt, FkGraphicRenderAtom::dispatchNext);
}

void FkGraphicRenderAtom::onConnect(std::shared_ptr<FkConnectChain> chain) {
    chain->next<FkPreRenderQuark>()
            ->next<FkLayerRenderQuark>();
}

FkResult FkGraphicRenderAtom::onCreate() {
    return FkSimpleAtom::onCreate();
}

FkResult FkGraphicRenderAtom::onDestroy() {
    return FkSimpleAtom::onDestroy();
}

FkResult FkGraphicRenderAtom::onStart() {
    return FkSimpleAtom::onStart();
}

FkResult FkGraphicRenderAtom::onStop() {
    return FkSimpleAtom::onStop();
}
