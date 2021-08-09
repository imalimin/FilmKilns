/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicVBOQuark.h"
#include "FkRenderRequestPrt.h"
#include "FkVertexComponent.h"
#include "FkCoordinateComponent.h"
#include "FkVertexObjectComponent.h"
#include "FkGLDefinition.h"

FkGraphicVBOQuark::FkGraphicVBOQuark() : FkQuark() {
    FK_MARK_SUPER
}

FkGraphicVBOQuark::~FkGraphicVBOQuark() {

}

void FkGraphicVBOQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderRequestPrt, FkGraphicVBOQuark::_onRenderRequest);
}

FkResult FkGraphicVBOQuark::onCreate() {
    auto ret = FkQuark::onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    allocator = std::make_shared<FkVertexObjectAllocator>();
    return ret;
}

FkResult FkGraphicVBOQuark::onDestroy() {
    allocator->release();
    return FkQuark::onDestroy();
}

FkResult FkGraphicVBOQuark::onStart() {
    return FkQuark::onStart();
}

FkResult FkGraphicVBOQuark::onStop() {
    return FkQuark::onStop();
}

FkResult FkGraphicVBOQuark::_onRenderRequest(std::shared_ptr<FkProtocol> p) {
    auto proto = Fk_POINTER_CAST(FkRenderRequestPrt, p);
    for (auto &layer : proto->req->layers) {
        auto vComp = layer->findComponent<FkVertexComponent>();
        FkAssert(nullptr != vComp, FK_FAIL);
        auto cComp = layer->findComponent<FkCoordinateComponent>();
        FkAssert(nullptr != cComp, FK_FAIL);

        FkVBODescription desc(vComp->getSize() + cComp->getSize());
        auto comp = std::make_shared<FkVertexObjectComponent>();
        comp->vbo = allocator->alloc(desc);
        FkAssert(nullptr != comp->vbo, FK_FAIL);

        int offset = 0;
        glBufferSubData(GL_ARRAY_BUFFER, offset, vComp->getSize(),
                        reinterpret_cast<const void *>(vComp->getSize()));
        offset += vComp->getSize();
        glBufferSubData(GL_ARRAY_BUFFER, offset, cComp->getSize(),
                        reinterpret_cast<const void *>(cComp->getSize()));
        layer->addComponent(comp);
    }
    return FK_OK;
}
