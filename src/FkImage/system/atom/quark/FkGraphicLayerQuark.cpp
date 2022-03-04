/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicLayerQuark.h"
#include "FkGraphicNewLayerPrt.h"
#include "FkGraphicUpdateLayerPrt.h"
#include "FkColorComponent.h"
#include "FkSizeComponent.h"
#include "FkTexIDComponent.h"
#include "FkRenderRequestPrt.h"
#include "FkTransComponent.h"
#include "FkScaleComponent.h"
#include "FkRotateComponent.h"
#include "FkLayerPostTransProto.h"
#include "FkLayerPostScaleProto.h"
#include "FkLayerPostRotateProto.h"
#include "FkScaleTypeComponent.h"
#include "FkMeasureTransProto.h"
#include "FkDrawPointProto.h"
#include "FkPointComponent.h"
#include "FkVertexComponent.h"
#include "FkCoordinateComponent.h"
#include "FkRenderContext.h"

//每个点占多少字节
#define SIZE_OF_VERTEX  sizeof(float)
//多少个坐标
#define COUNT_VERTEX  4
//每个坐标的维度
#define COUNT_PER_VERTEX  2

FkGraphicLayerQuark::FkGraphicLayerQuark() : FkQuark() {
    FK_MARK_SUPER
}

FkGraphicLayerQuark::~FkGraphicLayerQuark() {

}

void FkGraphicLayerQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicNewLayerPrt, FkGraphicLayerQuark::_onNewLayer);
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicUpdateLayerPrt, FkGraphicLayerQuark::_onUpdateLayer);
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderRequestPrt, FkGraphicLayerQuark::_onRenderRequest);
    FK_PORT_DESC_QUICK_ADD(desc, FkLayerPostTransProto, FkGraphicLayerQuark::_onPostTranslate);
    FK_PORT_DESC_QUICK_ADD(desc, FkLayerPostScaleProto, FkGraphicLayerQuark::_onPostScale);
    FK_PORT_DESC_QUICK_ADD(desc, FkLayerPostRotateProto, FkGraphicLayerQuark::_onPostRotate);
    FK_PORT_DESC_QUICK_ADD(desc, FkMeasureTransProto, FkGraphicLayerQuark::_onMeasureTrans);
    FK_PORT_DESC_QUICK_ADD(desc, FkDrawPointProto, FkGraphicLayerQuark::_onDrawPoint);
}

FkResult FkGraphicLayerQuark::onCreate() {
    return FkQuark::onCreate();
}

FkResult FkGraphicLayerQuark::onDestroy() {
    layers.clear();
    return FkQuark::onDestroy();
}

FkResult FkGraphicLayerQuark::onStart() {
    return FkQuark::onStart();
}

FkResult FkGraphicLayerQuark::onStop() {
    return FkQuark::onStop();
}

void FkGraphicLayerQuark::_setupVertex(std::shared_ptr<FkGraphicLayer> layer) {
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

FkResult FkGraphicLayerQuark::_onNewLayer(std::shared_ptr<FkProtocol> p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkGraphicNewLayerPrt, p);
    proto->layer = newLayerEntity();
    if (proto->layer) {
        layers.emplace(std::make_pair(proto->layer->id, proto->layer));
        return FK_OK;
    }
    return FK_FAIL;
}

std::shared_ptr<FkGraphicLayer> FkGraphicLayerQuark::newLayerEntity() {
    auto layer = std::make_shared<FkGraphicLayer>();
    layer->addComponent(std::make_shared<FkTransComponent>());
    layer->addComponent(std::make_shared<FkScaleComponent>());
    layer->addComponent(std::make_shared<FkRotateComponent>());

    auto renderEngine = FkRenderContext::wrap(getContext())->getRenderEngine();
    FkAssert(renderEngine != nullptr, nullptr);
    layer->material = renderEngine->newMaterial();
    layer->id = layer->material->id();
    return layer;
}

FkResult FkGraphicLayerQuark::_onUpdateLayer(std::shared_ptr<FkProtocol> p) {
    auto proto = Fk_POINTER_CAST(FkGraphicUpdateLayerPrt, p);
    auto itr = layers.find(proto->layer->id);
    if (layers.end() == itr) {
        return FK_SKIP;
    }
    auto layer = itr->second;
    auto colorComp = proto->layer->findComponent<FkColorComponent>();
    if (nullptr != colorComp) {
        layer->addComponent(colorComp);
    } else {
        colorComp = std::make_shared<FkColorComponent>();
        colorComp->color = FkColor::white();
    }
    auto sizeComp = proto->layer->findComponent<FkSizeComponent>();
    if (nullptr != sizeComp) {
        auto layerSizeComp = layer->findComponent<FkSizeComponent>();
        if (nullptr != layerSizeComp) {
            layerSizeComp->size = sizeComp->size;
        } else {
            layer->addComponent(sizeComp);
            auto scaleComp = layer->findComponent<FkScaleComponent>();
            if (!proto->winSize.isZero() &&  nullptr != scaleComp) {
                scaleComp->value.x = FkGraphicLayer::calcScaleWithScaleType(layer,
                                                                            proto->scaleType,
                                                                            proto->winSize);
                scaleComp->value.y = scaleComp->value.x;
                scaleComp->value.z = 1.0f;
            }
        }

        auto renderEngine = FkRenderContext::wrap(getContext())->getRenderEngine();
        FkAssert(renderEngine != nullptr, nullptr);
        renderEngine->updateMaterial(layer->material, layerSizeComp->size, colorComp->color);
    }

    if (Fk_CANVAS_ID == layer->id) {
        auto canvasSize = layer->findComponent<FkSizeComponent>();
        if (nullptr != canvasSize) {
            proto->winSize = canvasSize->size;
        }
    }
    return FK_OK;
}

FkResult FkGraphicLayerQuark::_onRenderRequest(std::shared_ptr<FkProtocol> p) {
    auto prt = std::static_pointer_cast<FkRenderRequestPrt>(p);
    for (auto &it : layers) {
        prt->req->layers.emplace_back(std::make_shared<FkGraphicLayer>(*it.second));
    }
    return FK_OK;
}

FkResult FkGraphicLayerQuark::_onPostTranslate(std::shared_ptr<FkProtocol> p) {
    auto proto = Fk_POINTER_CAST(FkLayerPostTransProto, p);
    auto itr = layers.find(proto->layer);
    FkAssert(layers.end() != itr, FK_FAIL);

    auto comp = itr->second->findComponent<FkTransComponent>();
    FkAssert(nullptr != comp, FK_FAIL);
    comp->value += proto->value;
//    FkLogI(FK_DEF_TAG, "(%d, %d), (%d, %d)", comp->value.x, comp->value.y, proto->value.x, proto->value.y);
    return FK_OK;
}

FkResult FkGraphicLayerQuark::_onPostScale(std::shared_ptr<FkProtocol> p) {
    auto proto = Fk_POINTER_CAST(FkLayerPostScaleProto, p);
    auto itr = layers.find(proto->layer);
    FkAssert(layers.end() != itr, FK_FAIL);

    auto comp = itr->second->findComponent<FkScaleComponent>();
    FkAssert(nullptr != comp, FK_FAIL);
    comp->value.x *= proto->value.x;
    comp->value.y *= proto->value.y;
    comp->value.z *= proto->value.z;
    return FK_OK;
}

FkResult FkGraphicLayerQuark::_onPostRotate(std::shared_ptr<FkProtocol> p) {
    auto proto = Fk_POINTER_CAST(FkLayerPostRotateProto, p);
    auto itr = layers.find(proto->layer);
    FkAssert(layers.end() != itr, FK_FAIL);

    auto comp = itr->second->findComponent<FkRotateComponent>();
    FkAssert(nullptr != comp, FK_FAIL);
    auto result = comp->value.toDouble() + proto->value.toDouble();
    comp->value.num = result * 10000;
    comp->value.den = 10000;
    return FK_OK;
}

FkResult FkGraphicLayerQuark::_onMeasureTrans(std::shared_ptr<FkProtocol> p) {
    auto proto = Fk_POINTER_CAST(FkMeasureTransProto, p);
    auto itr = layers.find(proto->layerId);
    FkAssert(layers.end() != itr, FK_FAIL);
    proto->layer = std::make_shared<FkGraphicLayer>(*itr->second);
    return FK_OK;
}

FkResult FkGraphicLayerQuark::_onDrawPoint(std::shared_ptr<FkProtocol> p) {
    auto proto = Fk_POINTER_CAST(FkDrawPointProto, p);
    auto itr = layers.find(proto->layer);
    if (layers.end() != itr) {
        auto comp = std::make_shared<FkPointComponent>();
        comp->value = proto->value;
        comp->color = proto->color;
        itr->second->addComponent(comp);
    }
    return FK_OK;
}