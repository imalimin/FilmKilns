/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-27 13:47:00
*/

#include "FkRenderVboQuark.h"
#include "FkRenderProto.h"
#include "FkTexEntity.h"
#include "FkVertexCompo.h"
#include "FkCoordinateCompo.h"
#include "FkVboCompo.h"
#include "FkPointFCompo.h"

FK_IMPL_CLASS_TYPE(FkRenderVboQuark, FkQuark)

FkRenderVboQuark::FkRenderVboQuark() : FkQuark() {

}

FkRenderVboQuark::~FkRenderVboQuark() {

}

void FkRenderVboQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderProto, FkRenderVboQuark::_onRender);
}

FkResult FkRenderVboQuark::onCreate() {
    return FkQuark::onCreate();
}

FkResult FkRenderVboQuark::onDestroy() {
    return FkQuark::onDestroy();
}

FkResult FkRenderVboQuark::onStart() {
    auto ret = FkQuark::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    allocator = std::make_shared<FkVertexObjectAllocator>(4 * 2 * sizeof(float) * 10);
    return ret;
}

FkResult FkRenderVboQuark::onStop() {
    allocator->release();
    allocator = nullptr;
    return FkQuark::onStop();
}

FkResult FkRenderVboQuark::_onRender(std::shared_ptr<FkProtocol> &p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkRenderProto, p);
    auto vertexCompo =  FK_FIND_COMPO(proto->materials, FkVertexCompo);
    auto coordCompo =  FK_FIND_COMPO(proto->materials, FkCoordinateCompo);
    if (vertexCompo && coordCompo) {
        FkVBODescription desc(vertexCompo->size() + coordCompo->size());
        auto comp = std::make_shared<FkVboCompo>();
        comp->setup(allocator->alloc(desc),
                    static_cast<float *>(vertexCompo->data()), vertexCompo->desc,
                    static_cast<float *>(coordCompo->data()), coordCompo->desc);
        proto->materials->addComponent(comp);
    } else if (vertexCompo) {
        FkVBODescription desc(vertexCompo->size());
        auto comp = std::make_shared<FkVboCompo>();
        comp->setup(allocator->alloc(desc),
                    static_cast<float *>(vertexCompo->data()), vertexCompo->desc);
        proto->materials->addComponent(comp);
    }
    return FK_OK;
}