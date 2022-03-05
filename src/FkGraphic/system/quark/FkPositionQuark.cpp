/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-27 23:30:48
*/

#include "FkPositionQuark.h"
#include "FkRenderProto.h"
#include "FkTexEntity.h"
#include "FkVertexCompo.h"
#include "FkCoordinateCompo.h"

//每个点占多少字节
#define SIZE_OF_VERTEX  sizeof(float)
//多少个坐标
#define COUNT_VERTEX  4
//每个坐标的维度
#define COUNT_PER_VERTEX  2

FkPositionQuark::FkPositionQuark() : FkQuark() {
    FK_MARK_SUPER
}

FkPositionQuark::~FkPositionQuark() {

}

void FkPositionQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderProto, FkPositionQuark::_onRender);
}

FkResult FkPositionQuark::onCreate() {
    return FkQuark::onCreate();
}

FkResult FkPositionQuark::onDestroy() {
    return FkQuark::onDestroy();
}

FkResult FkPositionQuark::onStart() {
    auto ret = FkQuark::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    return FkQuark::onStart();
}

FkResult FkPositionQuark::onStop() {
    return FkQuark::onStop();
}

FkResult FkPositionQuark::_onRender(std::shared_ptr<FkProtocol> &p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkRenderProto, p);
    FK_CAST_NULLABLE_PTR_RETURN_INT(material, FkTexEntity, proto->materials);
    auto srcSize = material->size();
    float pos[]{
            -srcSize.getWidth() / 2.0f, -srcSize.getHeight() / 2.0f,//LEFT,BOTTOM
            srcSize.getWidth() / 2.0f, -srcSize.getHeight() / 2.0f,//RIGHT,BOTTOM
            -srcSize.getWidth() / 2.0f, srcSize.getHeight() / 2.0f,//LEFT,TOP
            srcSize.getWidth() / 2.0f, srcSize.getHeight() / 2.0f//RIGHT,TOP
    };
    float coordinate[]{
            0.0f, 0.0f,//LEFT,BOTTOM
            1.0f, 0.0f,//RIGHT,BOTTOM
            0.0f, 1.0f,//LEFT,TOP
            1.0f, 1.0f//RIGHT,TOP
    };
    auto vertex = std::make_shared<FkVertexCompo>();
    auto coord = std::make_shared<FkCoordinateCompo>();
    vertex->setup(COUNT_VERTEX, COUNT_PER_VERTEX, SIZE_OF_VERTEX, pos);
    coord->setup(COUNT_VERTEX, COUNT_PER_VERTEX, SIZE_OF_VERTEX, coordinate);
    material->addComponent(vertex);
    material->addComponent(coord);
    return FK_OK;
}