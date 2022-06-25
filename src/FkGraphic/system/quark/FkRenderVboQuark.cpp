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

//每个点占多少字节
#define SIZE_OF_VERTEX  sizeof(float)
//多少个坐标
#define COUNT_VERTEX  4
//每个坐标的维度
#define COUNT_PER_VERTEX  2

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
    auto material = std::dynamic_pointer_cast<FkTexEntity>(proto->materials);
    std::shared_ptr<FkVertexCompo> vertexCompo = nullptr;
    if (material) {
        auto size = material->texArray()->size;
        float pos[]{
                -size.getWidth() / 2.0f, -size.getHeight() / 2.0f,//LEFT,BOTTOM
                size.getWidth() / 2.0f, -size.getHeight() / 2.0f,//RIGHT,BOTTOM
                -size.getWidth() / 2.0f, size.getHeight() / 2.0f,//LEFT,TOP
                size.getWidth() / 2.0f, size.getHeight() / 2.0f//RIGHT,TOP
        };
        vertexCompo = std::make_shared<FkVertexCompo>();
        vertexCompo->setup(COUNT_VERTEX, COUNT_PER_VERTEX, SIZE_OF_VERTEX, pos);
    }
    float coordinate[]{
            0.0f, 0.0f,//LEFT,BOTTOM
            1.0f, 0.0f,//RIGHT,BOTTOM
            0.0f, 1.0f,//LEFT,TOP
            1.0f, 1.0f//RIGHT,TOP
    };
    auto coordCompo = std::make_shared<FkCoordinateCompo>();
    coordCompo->setup(COUNT_VERTEX, COUNT_PER_VERTEX, SIZE_OF_VERTEX, coordinate);

    FkVBODescription desc(vertexCompo->size() + coordCompo->size());
    auto comp = std::make_shared<FkVboCompo>();
    comp->setup(allocator->alloc(desc),
                static_cast<float *>(vertexCompo->data()), vertexCompo->desc,
                static_cast<float *>(coordCompo->data()), coordCompo->desc);
    proto->materials->addComponent(comp);

    return FK_OK;
}