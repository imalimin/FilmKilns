/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkRenderModelAtom.h"
#include "FkIDQuark.h"
#include "FkRenderDefine.h"
#include "FkVertexCompo.h"
#include "FkCoordinateCompo.h"
#include "FkRenderMvpQuark.h"

//每个点占多少字节
#define SIZE_OF_VERTEX  sizeof(float)
//多少个坐标
#define COUNT_VERTEX  4
//每个坐标的维度
#define COUNT_PER_VERTEX  2

FkRenderModelAtom::FkRenderModelAtom() : FkSimpleMolecule() {
    FK_MARK_SUPER
}

FkRenderModelAtom::~FkRenderModelAtom() {

}

void FkRenderModelAtom::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderProto, FkRenderModelAtom::_onRender);
    FK_PORT_DESC_QUICK_ADD(desc, FkGenIDProto, FkRenderModelAtom::dispatchNext);
}

void FkRenderModelAtom::onConnect(std::shared_ptr<FkConnectChain> chain) {
    chain->next<FkIDQuark>()
            ->next<FkRenderMvpQuark>();
}

FkResult FkRenderModelAtom::onCreate() {
    auto ret = FkSimpleMolecule::onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkRenderModelAtom::onDestroy() {
    auto ret = FkSimpleMolecule::onDestroy();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkRenderModelAtom::onStart() {
    auto ret = FkSimpleMolecule::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkRenderModelAtom::onStop() {
    auto ret = FkSimpleMolecule::onStop();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkRenderModelAtom::_onRender(std::shared_ptr<FkProtocol> &p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkRenderProto, p);
    if (!FK_INSTANCE_OF(proto->device, FkTexDeviceEntity)) {
        return FK_SKIP;
    }
    FK_CAST_NULLABLE_PTR_RETURN_INT(device, FkTexDeviceEntity, proto->device);
    FK_CAST_NULLABLE_PTR_RETURN_INT(material, FkTexEntity, proto->material);
    auto srcTexCompo = material->tex();
    auto fboCompo = material->fbo();
    auto dstTexCompo = device->tex();

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
    return dispatchNext(p);
}