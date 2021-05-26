/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicTexQuark.h"
#include "FkGraphicLayerPrt.h"
#include "FkGraphicNewTexPtl.h"
#include "FkTexComponent.h"
#include "FkGraphicTexComponent.h"
#include "EGL/egl.h"

FkGraphicTexQuark::FkGraphicTexQuark() : FkQuark() {
    FK_MARK_SUPER
}

FkGraphicTexQuark::~FkGraphicTexQuark() {

}

void FkGraphicTexQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicLayerPrt, FkGraphicTexQuark::_onDrawLayer);
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicNewTexPtl, FkGraphicTexQuark::_onAllocTex);
}

FkResult FkGraphicTexQuark::onCreate() {
    auto ret = FkQuark::onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    allocator = std::make_shared<FkGraphicAllocator>();
    return ret;
}

FkResult FkGraphicTexQuark::onDestroy() {
    allocator->release();
    return FkQuark::onDestroy();
}

FkResult FkGraphicTexQuark::onStart() {
    return FkQuark::onStart();
}

FkResult FkGraphicTexQuark::onStop() {
    return FkQuark::onStop();
}

FkResult FkGraphicTexQuark::_onDrawLayer(std::shared_ptr<FkProtocol> p) {
    auto ptl = std::static_pointer_cast<FkGraphicLayerPrt>(p);
    std::vector<std::shared_ptr<FkGraphicComponent>> vec;
    if (FK_OK != ptl->layer->findComponent(vec, FkClassType::type<FkTexComponent>())) {
        return FK_FAIL;
    }
    for (auto it : vec) {
        auto tex = std::static_pointer_cast<FkTexComponent>(it);
        auto itr = sMap.find(tex->texId);
        if (itr != sMap.end()) {
            auto comp = std::make_shared<FkGraphicTexComponent>();
            comp->texture = itr->second;
            ptl->layer->addComponent(comp);
        }
    }
    return FK_OK;
}

FkResult FkGraphicTexQuark::_onAllocTex(std::shared_ptr<FkProtocol> p) {
    auto ptl = std::static_pointer_cast<FkGraphicTexPtl>(p);
    FkTexDescription desc;
    auto tex = allocator->alloc(desc);
    if (nullptr == tex) {
        return FK_FAIL;
    }
    sMap.insert(std::make_pair(tex->id, tex));
    ptl->texId = tex->id;
    return FK_OK;
}
