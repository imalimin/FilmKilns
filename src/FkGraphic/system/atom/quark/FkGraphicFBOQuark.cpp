/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicFBOQuark.h"
#include "FkGraphicLayerPrt.h"
#include "FkGraphicFBOComponent.h"

FkGraphicFBOQuark::FkGraphicFBOQuark() : FkQuark() {
}

FkGraphicFBOQuark::~FkGraphicFBOQuark() {

}

void FkGraphicFBOQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicLayerPrt, FkGraphicFBOQuark::_onDrawLayer);
}

FkResult FkGraphicFBOQuark::onCreate() {
    auto ret = FkQuark::onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    allocator = std::make_shared<FkGraphicFBOAllocator>();
    return ret;
}

FkResult FkGraphicFBOQuark::onDestroy() {
    allocator->release();
    return FkQuark::onDestroy();
}

FkResult FkGraphicFBOQuark::onStart() {
    return FkQuark::onStart();
}

FkResult FkGraphicFBOQuark::onStop() {
    return FkQuark::onStop();
}

FkResult FkGraphicFBOQuark::_onDrawLayer(std::shared_ptr<FkProtocol> p) {
    auto ptl = std::static_pointer_cast<FkGraphicLayerPrt>(p);
    auto comp = std::make_shared<FkGraphicFBOComponent>();
    int32_t desc = 0;
    comp->fbo = allocator->alloc(desc);
    ptl->layer->addComponent(comp);
    return FK_OK;
}