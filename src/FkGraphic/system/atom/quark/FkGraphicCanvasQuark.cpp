/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicCanvasQuark.h"
#include "FkRenderRequestPrt.h"
#include "FkGraphicUpdateCanvasProto.h"
#include "FkSizeComponent.h"
#include "FkTexIDComponent.h"
#include "FkQuerySizeProto.h"
#include "FkTransComponent.h"
#include "FkScaleComponent.h"
#include "FkRotateComponent.h"
#include "FkScaleTypeComponent.h"
#include "FkMeasureTransProto.h"
#include "FkGraphicUpdateLayerPrt.h"
#include "FkDrawPointProto.h"
#include "FkPointComponent.h"
#include "FkVertexComponent.h"
#include "FkCoordinateComponent.h"

//每个点占多少字节
#define SIZE_OF_VERTEX  sizeof(float)
//多少个坐标
#define COUNT_VERTEX  4
//每个坐标的维度
#define COUNT_PER_VERTEX  2

FkGraphicCanvasQuark::FkGraphicCanvasQuark() : FkQuark() {
    FK_MARK_SUPER
}

FkGraphicCanvasQuark::~FkGraphicCanvasQuark() {

}

void FkGraphicCanvasQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderRequestPrt, FkGraphicCanvasQuark::_onRenderRequest);
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicUpdateCanvasProto, FkGraphicCanvasQuark::_onUpdate);
    FK_PORT_DESC_QUICK_ADD(desc, FkQuerySizeProto, FkGraphicCanvasQuark::_onQueryCanvasSize);
    FK_PORT_DESC_QUICK_ADD(desc, FkMeasureTransProto, FkGraphicCanvasQuark::_onMeasureTrans);
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicUpdateLayerPrt, FkGraphicCanvasQuark::_onWithCanvasSize);
    FK_PORT_DESC_QUICK_ADD(desc, FkDrawPointProto, FkGraphicCanvasQuark::_onDrawPoint);
}

FkResult FkGraphicCanvasQuark::onCreate() {
    canvas = std::make_shared<FkGraphicLayer>();
    canvas->id = Fk_CANVAS_ID;
    canvas->addComponent(std::make_shared<FkTransComponent>());
    canvas->addComponent(std::make_shared<FkScaleComponent>());
    canvas->addComponent(std::make_shared<FkRotateComponent>());
    return FkQuark::onCreate();
}

FkResult FkGraphicCanvasQuark::onDestroy() {
    return FkQuark::onDestroy();
}

FkResult FkGraphicCanvasQuark::onStart() {
    return FkQuark::onStart();
}

FkResult FkGraphicCanvasQuark::onStop() {
    return FkQuark::onStop();
}

void FkGraphicCanvasQuark::_setupVertex(std::shared_ptr<FkGraphicLayer> layer) {
    auto sizeComp = layer->findComponent<FkSizeComponent>();
    FkAssert(nullptr != sizeComp, );
    auto vertex = layer->findComponent<FkVertexComponent>();
    if (nullptr == vertex) {
        vertex = std::make_shared<FkVertexComponent>();
        layer->addComponent(vertex);
    }
    auto coord = layer->findComponent<FkCoordinateComponent>();
    if (nullptr == coord) {
        coord = std::make_shared<FkCoordinateComponent>();
        layer->addComponent(coord);
    }

    float pos[]{
            -sizeComp->size.getWidth() / 2.0f, -sizeComp->size.getHeight() / 2.0f,//LEFT,BOTTOM
            sizeComp->size.getWidth() / 2.0f, -sizeComp->size.getHeight() / 2.0f,//RIGHT,BOTTOM
            -sizeComp->size.getWidth() / 2.0f, sizeComp->size.getHeight() / 2.0f,//LEFT,TOP
            sizeComp->size.getWidth() / 2.0f, sizeComp->size.getHeight() / 2.0f//RIGHT,TOP
    };
    float coordinate[]{
            0.0f, 0.0f,//LEFT,BOTTOM
            1.0f, 0.0f,//RIGHT,BOTTOM
            0.0f, 1.0f,//LEFT,TOP
            1.0f, 1.0f//RIGHT,TOP
    };
    vertex->setup(COUNT_VERTEX, COUNT_PER_VERTEX, SIZE_OF_VERTEX, pos);
    coord->setup(COUNT_VERTEX, COUNT_PER_VERTEX, SIZE_OF_VERTEX, coordinate);
}

FkResult FkGraphicCanvasQuark::_onUpdate(std::shared_ptr<FkProtocol> p) {
    auto proto = Fk_POINTER_CAST(FkGraphicUpdateCanvasProto, p);
    auto sizeComp = proto->layer->findComponent<FkSizeComponent>();
    if (nullptr != sizeComp) {
        auto canvasSize = canvas->findComponent<FkSizeComponent>();
        if (nullptr != canvasSize) {
            canvasSize->size = sizeComp->size;
        } else {
            canvas->addComponent(sizeComp);
            auto scaleComp = canvas->findComponent<FkScaleComponent>();
            FkAssert(nullptr != scaleComp, FK_FAIL);
            scaleComp->value.x = FkGraphicLayer::calcScaleWithScaleType(canvas, proto->scaleType, proto->winSize);
            scaleComp->value.y = scaleComp->value.x;
            scaleComp->value.z = 1.0f;
        }
    }
    auto texIDComp = proto->layer->findComponent<FkTexIDComponent>();
    if (nullptr != texIDComp) {
        canvas->addComponent(texIDComp);
    }
    _setupVertex(canvas);
    return FK_OK;
}

FkResult FkGraphicCanvasQuark::_onRenderRequest(std::shared_ptr<FkProtocol> p) {
    auto prt = std::static_pointer_cast<FkRenderRequestPrt>(p);
    FkAssert(nullptr != prt->req, FK_EMPTY_DATA);
    prt->req->layers.emplace_back(std::make_shared<FkGraphicLayer>(*canvas));
    return FK_OK;
}

FkResult FkGraphicCanvasQuark::_onQueryCanvasSize(std::shared_ptr<FkProtocol> p) {
    auto proto = std::static_pointer_cast<FkQuerySizeProto>(p);
    std::vector<std::shared_ptr<FkGraphicComponent>> vec;
    if (FK_OK == canvas->findComponent(vec, FkClassType::type<FkSizeComponent>())) {
        auto sizeComp = Fk_POINTER_CAST(FkSizeComponent, vec[0]);
        proto->value = sizeComp->size;
    }
    vec.clear();
    return FK_OK;
}

FkResult FkGraphicCanvasQuark::_onMeasureTrans(std::shared_ptr<FkProtocol> p) {
    auto proto = Fk_POINTER_CAST(FkMeasureTransProto, p);
    proto->canvas = std::make_shared<FkGraphicLayer>(*canvas);
    return FK_OK;
}

FkResult FkGraphicCanvasQuark::_onWithCanvasSize(std::shared_ptr<FkProtocol> p) {
    auto proto = Fk_POINTER_CAST(FkGraphicUpdateLayerPrt, p);
    auto canvasSize = canvas->findComponent<FkSizeComponent>();
    if (nullptr != canvasSize) {
        proto->winSize = canvasSize->size;
    }
    return FK_OK;
}

FkResult FkGraphicCanvasQuark::_onDrawPoint(std::shared_ptr<FkProtocol> p) {
    auto proto = Fk_POINTER_CAST(FkDrawPointProto, p);
    if (proto->layer == canvas->id) {
        auto comp = std::make_shared<FkPointComponent>();
        comp->value = proto->value;
        comp->color = proto->color;
        canvas->addComponent(comp);
    }
    return FK_OK;
}
