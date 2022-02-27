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

FkRenderVboQuark::FkRenderVboQuark() : FkQuark() {
    FK_MARK_SUPER
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
    allocator = std::make_shared<FkVertexObjectAllocator>();
    return FkQuark::onStart();
}

FkResult FkRenderVboQuark::onStop() {
    allocator->release();
    return FkQuark::onStop();
}

FkResult FkRenderVboQuark::_onRender(std::shared_ptr<FkProtocol> &p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkRenderProto, p);
    auto vertexCompo =  proto->material->findComponent<FkVertexCompo>();
    auto coordCompo =  proto->material->findComponent<FkCoordinateCompo>();
    if (vertexCompo && coordCompo) {
        FkVBODescription desc(vertexCompo->size() + coordCompo->size());
        auto comp = std::make_shared<FkVboCompo>();
        comp->setup(allocator->alloc(desc),
                    static_cast<float *>(vertexCompo->data()), vertexCompo->desc,
                    static_cast<float *>(coordCompo->data()), coordCompo->desc);
        proto->material->addComponent(comp);
    }
    return FK_OK;
}