/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicSourceAtom.h"
#include "FkGraphicContextQuark.h"
#include "FkGraphicTexQuark.h"
#include "FkGraphicFBOQuark.h"
#include "FkGraphicProgramQuark.h"
#include "FkGraphicNewTexPtl.h"
#include "FkSetSurfacePrt.h"
#include "FkRenderRequestPrt.h"
#include "FkGraphicUpdateTexPrt.h"
#include "FkGraphicTexDelPtl.h"
#include "FkUpdateTexWithBmpPrt.h"
#include "FkGraphicVBOQuark.h"
#include "FkRenderContext.h"
#include "FkEmptyQuark.h"

FkGraphicSourceAtom::FkGraphicSourceAtom() : FkSimpleAtom() {
    FK_MARK_SUPER
}

FkGraphicSourceAtom::~FkGraphicSourceAtom() {

}

void FkGraphicSourceAtom::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkSetSurfacePrt, FkGraphicSourceAtom::_onSetSurface);
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicUpdateTexPrt, FkGraphicSourceAtom::dispatchNext);
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicNewTexPtl, FkGraphicSourceAtom::dispatchNext);
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicTexDelPtl, FkGraphicSourceAtom::dispatchNext);
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderRequestPrt, FkGraphicSourceAtom::dispatchNext);
    FK_PORT_DESC_QUICK_ADD(desc, FkUpdateTexWithBmpPrt, FkGraphicSourceAtom::dispatchNext);
}

void FkGraphicSourceAtom::onConnect(std::shared_ptr<FkConnectChain> chain) {
//    chain->next<FkGraphicContextQuark>()
//            ->next<FkGraphicTexQuark>()
//            ->next<FkGraphicFBOQuark>()
//            ->next<FkGraphicVBOQuark>()
//            ->next<FkGraphicProgramQuark>();
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
