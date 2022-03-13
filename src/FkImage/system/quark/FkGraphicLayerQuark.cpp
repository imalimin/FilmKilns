/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicLayerQuark.h"
#include "FkGraphicNewLayerPrt.h"
#include "FkGraphicUpdateLayerPrt.h"
#include "FkColorCompo.h"
#include "FkSizeCompo.h"
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
#include "FkVertexCompo.h"
#include "FkCoordinateCompo.h"
#include "FkRenderContext.h"
#include "FkBitmapCompo.h"
#include "FkFilePathCompo.h"
#include "FkQueryLayersProto.h"

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
    FK_PORT_DESC_QUICK_ADD(desc, FkQueryLayersProto, FkGraphicLayerQuark::_onQueryLayers);
}

FkResult FkGraphicLayerQuark::onCreate() {
    return FkQuark::onCreate();
}

FkResult FkGraphicLayerQuark::onDestroy() {
    return FkQuark::onDestroy();
}

FkResult FkGraphicLayerQuark::onStart() {
    return FkQuark::onStart();
}

FkResult FkGraphicLayerQuark::onStop() {
    layers.clear();
    return FkQuark::onStop();
}

void FkGraphicLayerQuark::_setupVertex(std::shared_ptr<FkGraphicLayer> layer) {
    auto sizeComp = layer->findComponent<FkSizeCompo>();
    FkAssert(nullptr != sizeComp, );
    auto vertex = layer->findComponent<FkVertexCompo>();
    if (nullptr == vertex) {
        vertex = std::make_shared<FkVertexCompo>();
        layer->addComponent(vertex);
    }
    auto coord = layer->findComponent<FkCoordinateCompo>();
    if (nullptr == coord) {
        coord = std::make_shared<FkCoordinateCompo>();
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

bool FkGraphicLayerQuark::_isExistLayer(FkID id) {
    return layers.find(id) != layers.end();
}

FkResult FkGraphicLayerQuark::_onNewLayer(std::shared_ptr<FkProtocol> p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkGraphicNewLayerPrt, p);
    proto->layer = newLayerEntity();
    if (FK_ID_NONE != proto->expectId && !_isExistLayer(proto->expectId)) {
        proto->layer->id = proto->expectId;
    }
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
    layer->material = renderEngine->addMaterial();
    layer->id = layer->material->id();
    return layer;
}

FkResult FkGraphicLayerQuark::_onUpdateLayer(std::shared_ptr<FkProtocol> p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkGraphicUpdateLayerPrt, p);
    auto itr = layers.find(proto->layer->id);
    if (layers.end() == itr) {
        itr = layers.find(Fk_CANVAS_ID);
        if (layers.end() != itr) {
            auto canvas = itr->second;
            auto canvasSizeCompo = canvas->findComponent<FkSizeCompo>();
            if (nullptr != canvasSizeCompo) {
                proto->winSize = canvasSizeCompo->size;
            }
        }
        return FK_SKIP;
    }
    FkResult ret = FK_OK;
    auto layer = itr->second;
    auto colorComp = proto->layer->findComponent<FkColorCompo>();
    if (nullptr != colorComp) {
        layer->addComponent(colorComp);
    } else {
        colorComp = std::make_shared<FkColorCompo>(FkColor::black());
    }
    auto sizeComp = proto->layer->findComponent<FkSizeCompo>();
    if (nullptr != sizeComp) {
        auto layerSizeComp = layer->findComponent<FkSizeCompo>();
        if (layerSizeComp == nullptr) {
            layerSizeComp = std::make_shared<FkSizeCompo>(sizeComp->size);
            layer->addComponent(layerSizeComp);
            auto scaleComp = layer->findComponent<FkScaleComponent>();
            if (!proto->winSize.isZero() &&  nullptr != scaleComp) {
                scaleComp->value.x = FkGraphicLayer::calcScaleWithScaleType(layer,
                                                                            proto->scaleType,
                                                                            proto->winSize);
                scaleComp->value.y = scaleComp->value.x;
                scaleComp->value.z = 1.0f;
            }
        }
        layerSizeComp->size = sizeComp->size;
        auto renderEngine = FkRenderContext::wrap(getContext())->getRenderEngine();
        FkAssert(renderEngine != nullptr, nullptr);
        auto bmpCompo = proto->layer->findComponent<FkBitmapCompo>();
        if (bmpCompo) {
            auto buf = FkBuffer::alloc(bmpCompo->bmp->getByteSize());
            memcpy(buf->data(), bmpCompo->bmp->getPixels(), buf->capacity());
            ret = renderEngine->updateMaterialWithBitmap(layer->material,
                                                         layerSizeComp->size,
                                                         buf);
            FkAssert(FK_OK == ret, ret);
            layer->copyComponentFrom<FkFilePathCompo>(proto->layer);
        } else {
            ret = renderEngine->updateMaterial(layer->material,
                                               layerSizeComp->size,
                                               colorComp->color);
            FkAssert(FK_OK == ret, ret);
        }
    }
    _setupVertex(itr->second);
    return ret;
}

FkResult FkGraphicLayerQuark::_onRenderRequest(std::shared_ptr<FkProtocol> p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkRenderRequestPrt, p);
    for (auto &it : layers) {
        proto->req->layers.emplace_back(std::make_shared<FkGraphicLayer>(*it.second));
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
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkLayerPostRotateProto, p);
    auto itr = layers.find(proto->layer);
    FkAssert(layers.end() != itr, FK_FAIL);

    auto comp = itr->second->findComponent<FkRotateComponent>();
    FkAssert(nullptr != comp, FK_FAIL);
    auto result = comp->value.toDouble() + proto->value.toDouble();
    comp->value.num = result * 100000000;
    comp->value.den = 100000000;
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

FkResult FkGraphicLayerQuark::_onQueryLayers(std::shared_ptr<FkProtocol> p) {
    auto proto = Fk_POINTER_CAST(FkQueryLayersProto, p);
    for (auto &itr : layers) {
        proto->layers.emplace_back(std::make_shared<FkGraphicLayer>(*(itr.second)));
    }
    return FK_OK;
}