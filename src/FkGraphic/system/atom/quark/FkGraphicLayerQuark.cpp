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
#include "FkTexComponent.h"
#include "FkRenderRequestPrt.h"
#include "FkTransComponent.h"
#include "FkScaleComponent.h"
#include "FkRotateComponent.h"
#include "FkLayerPostTransProto.h"
#include "FkLayerPostScaleProto.h"
#include "FkLayerPostRotateProto.h"
#include "FkScaleTypeComponent.h"
#include "FkMeasureTransProto.h"

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

FkResult FkGraphicLayerQuark::_onNewLayer(std::shared_ptr<FkProtocol> p) {
    auto proto = Fk_POINTER_CAST(FkGraphicNewLayerPrt, p);
    auto layer = std::make_shared<FkGraphicLayer>();
    layer->addComponent(std::make_shared<FkTransComponent>());
    layer->addComponent(std::make_shared<FkScaleComponent>());
    layer->addComponent(std::make_shared<FkRotateComponent>());
    std::lock_guard<std::mutex> guard(mtx);
    ++mCurID;
    layer->id = mCurID;
    proto->layer = layer;
    layers.emplace(std::make_pair(mCurID, layer));
    return FK_OK;
}

FkResult FkGraphicLayerQuark::_onUpdateLayer(std::shared_ptr<FkProtocol> p) {
    auto proto = Fk_POINTER_CAST(FkGraphicUpdateLayerPrt, p);
    auto itr = layers.find(proto->layer->id);
    if (layers.end() == itr) {
        return FK_FAIL;
    }
    auto colorComp = proto->layer->findComponent<FkColorComponent>();
    if (nullptr != colorComp) {
        itr->second->addComponent(colorComp);
    }
    auto sizeComp = proto->layer->findComponent<FkSizeComponent>();
    if (nullptr != sizeComp) {
        auto layerSizeComp = itr->second->findComponent<FkSizeComponent>();
        if (nullptr != layerSizeComp) {

        } else {
            itr->second->addComponent(sizeComp);
            auto scaleComp = itr->second->findComponent<FkScaleComponent>();
            if (!proto->winSize.isZero() &&  nullptr != scaleComp) {
                scaleComp->value.x = FkGraphicLayer::calcScaleWithScaleType(itr->second,
                                                                            proto->scaleType,
                                                                            proto->winSize);
                scaleComp->value.y = scaleComp->value.x;
                scaleComp->value.z = 1.0f;
            }
        }
    }
    auto texComp = proto->layer->findComponent<FkTexComponent>();
    if (nullptr != texComp) {
        itr->second->addComponent(texComp);
    }
    return FK_OK;
}

FkResult FkGraphicLayerQuark::_onRenderRequest(std::shared_ptr<FkProtocol> p) {
    auto prt = std::static_pointer_cast<FkRenderRequestPrt>(p);
    if (nullptr == prt->req) {
        prt->req = std::make_shared<FkRenderRequest>();
    }
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