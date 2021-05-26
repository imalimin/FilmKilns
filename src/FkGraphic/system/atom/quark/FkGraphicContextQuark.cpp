/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicContextQuark.h"
#include "FkGraphicLayerPrt.h"
#include "FkGraphicNewTexPtl.h"

#define TAG "FkGraphicContextQuark"

FkGraphicContextQuark::FkGraphicContextQuark() : FkQuark() {
    FK_MARK_SUPER
}

FkGraphicContextQuark::~FkGraphicContextQuark() {

}

void FkGraphicContextQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicLayerPrt, FkGraphicContextQuark::_onMakeCurrent);
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicNewTexPtl, FkGraphicContextQuark::_onMakeCurrent);
}

FkResult FkGraphicContextQuark::onCreate() {
    auto ret = FkQuark::onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    context = std::make_shared<FkGraphicContext>(TAG);
    context->create();
    context->makeCurrent();
    return ret;
}

FkResult FkGraphicContextQuark::onDestroy() {
    context->destroy();
    return FkQuark::onDestroy();
}

FkResult FkGraphicContextQuark::onStart() {
    return FkQuark::onStart();
}

FkResult FkGraphicContextQuark::onStop() {
    return FkQuark::onStop();
}

FkResult FkGraphicContextQuark::_onMakeCurrent(std::shared_ptr<FkProtocol> p) {
    context->makeCurrent();
    if (FK_INSTANCE_OF(p, FkGraphicLayerPrt)) {
        auto ptl = std::static_pointer_cast<FkGraphicLayerPrt>(p);
        auto comp = std::make_shared<FkGraphicCtxComponent>();
        comp->context = context;
        ptl->layer->addComponent(comp);
    }
    auto ctx = eglGetCurrentContext();
    return FK_OK;
}
