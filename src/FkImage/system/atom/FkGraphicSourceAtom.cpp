/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicSourceAtom.h"
#include "FkSetSurfacePrt.h"
#include "FkRenderContext.h"
#include "FkEmptyQuark.h"

FK_IMPL_CLASS_TYPE(FkGraphicSourceAtom, FkSimpleAtom)

FkGraphicSourceAtom::FkGraphicSourceAtom() : FkSimpleAtom() {

}

FkGraphicSourceAtom::~FkGraphicSourceAtom() {

}

void FkGraphicSourceAtom::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkSetSurfacePrt, FkGraphicSourceAtom::_onSetSurface);
}

void FkGraphicSourceAtom::onConnect(std::shared_ptr<FkConnectChain> chain) {
    chain->next<FkEmptyQuark>();
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

FkResult FkGraphicSourceAtom::_onSetSurface(std::shared_ptr<FkProtocol> p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkSetSurfacePrt, p);
    auto renderEngine = FkRenderContext::wrap(getContext())->getRenderEngine();
    FkAssert(renderEngine != nullptr, FK_NPE);
    return renderEngine->updateWindow(proto->win);
}
