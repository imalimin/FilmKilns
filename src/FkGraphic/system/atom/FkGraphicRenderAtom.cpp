/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicRenderAtom.h"
#include "FkGraphicLayerPrt.h"

FkGraphicRenderAtom::FkGraphicRenderAtom() : FkAtom() {

}

FkGraphicRenderAtom::~FkGraphicRenderAtom() {

}

void FkGraphicRenderAtom::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicLayerPrt, FkGraphicRenderAtom::_onDrawLayer);
}

FkResult FkGraphicRenderAtom::onCreate() {
    return FkAtom::onCreate();
}

FkResult FkGraphicRenderAtom::onDestroy() {
    return FkAtom::onDestroy();
}

FkResult FkGraphicRenderAtom::onStart() {
    return FkAtom::onStart();
}

FkResult FkGraphicRenderAtom::onStop() {
    return FkAtom::onStop();
}

FkResult FkGraphicRenderAtom::_onDrawLayer(std::shared_ptr<FkProtocol> p) {
    return FK_OK;
}
