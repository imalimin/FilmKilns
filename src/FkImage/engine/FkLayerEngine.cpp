/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkLayerEngine.h"
#include "FkGraphicNewLayerPrt.h"
#include "FkGraphicUpdateLayerPrt.h"
#include "FkColorCompo.h"
#include "FkSizeCompo.h"
#include "FkSetSurfacePrt.h"
#include "FkRenderRequestPrt.h"
#include "FkQuerySizeProto.h"
#include "FkIntVec2.h"
#include "FkFloatVec3.h"
#include "FkLayerPostTransProto.h"
#include "FkLayerPostScaleProto.h"
#include "FkLayerPostRotateProto.h"
#include "FkMeasureTransProto.h"
#include "FkDrawPointProto.h"
#include "FkQueryLayersProto.h"
#include "FkRenderEngineCompo.h"

const FkID FkLayerEngine::FK_MSG_NEW_LAYER = 0x1;
const FkID FkLayerEngine::FK_MSG_UPDATE_LAYER_WITH_COLOR = 0x2;
const FkID FkLayerEngine::FK_MSG_SET_SURFACE = 0x3;
const FkID FkLayerEngine::FK_MSG_NOTIFY_RENDER = 0x4;
const FkID FkLayerEngine::FK_MSG_SET_CANVAS_SIZE = 0x5;
const FkID FkLayerEngine::FK_MSG_POST_TRANSLATE = 0x6;
const FkID FkLayerEngine::FK_MSG_POST_SCALE = 0x7;
const FkID FkLayerEngine::FK_MSG_POST_ROTATION = 0x8;
const FkID FkLayerEngine::FK_MSG_DRAW_POINT = 0x9;
const FkID FkLayerEngine::FK_MSG_QUERY_LAYERS = 0x10;

FkLayerEngine::FkLayerEngine(std::shared_ptr<FkEngine> &renderEngine, std::string name)
        : FkEngine(name), renderEngine(renderEngine) {
    FK_MARK_SUPER
    FK_REG_MSG(FK_MSG_NEW_LAYER, FkLayerEngine::_newLayer);
    FK_REG_MSG(FK_MSG_UPDATE_LAYER_WITH_COLOR, FkLayerEngine::_updateLayerWithColor);
    FK_REG_MSG(FK_MSG_SET_SURFACE, FkLayerEngine::_setSurface);
    FK_REG_MSG(FK_MSG_NOTIFY_RENDER, FkLayerEngine::_notifyRender);
    FK_REG_MSG(FK_MSG_SET_CANVAS_SIZE, FkLayerEngine::_setCanvasSize);
    FK_REG_MSG(FK_MSG_POST_TRANSLATE, FkLayerEngine::_postTranslate);
    FK_REG_MSG(FK_MSG_POST_SCALE, FkLayerEngine::_postScale);
    FK_REG_MSG(FK_MSG_POST_ROTATION, FkLayerEngine::_postRotation);
    FK_REG_MSG(FK_MSG_DRAW_POINT, FkLayerEngine::_drawPoint);
    FK_REG_MSG(FK_MSG_QUERY_LAYERS, FkLayerEngine::_queryLayers);
    client = std::make_shared<FkLocalClient>();
    molecule = std::make_shared<FkGraphicMolecule>();
}

FkLayerEngine::~FkLayerEngine() {

}

FkResult FkLayerEngine::onCreate() {
    auto ret = FkEngine::onCreate();
    FkAssert(ret == FK_OK, ret);
    ret = renderEngine->create();
    FkAssert(ret == FK_OK, ret);
    auto proto = std::make_shared<FkOnCreatePrt>();
    proto->context = std::make_shared<FkQuarkContext>();
    proto->context->addComponent(std::make_shared<FkRenderEngineCompo>(renderEngine));
    return client->with(molecule)->send(proto);
}

FkResult FkLayerEngine::onDestroy() {
    auto ret = FkEngine::onDestroy();
    FkAssert(ret == FK_OK, ret);
    ret = client->quickSend<FkOnDestroyPrt>(molecule);
    FkAssert(ret == FK_OK, ret);
    return renderEngine->destroy();
}

FkResult FkLayerEngine::onStart() {
    auto ret = FkEngine::onStart();
    FkAssert(ret == FK_OK, ret);
    ret = renderEngine->start();
    FkAssert(ret == FK_OK, ret);
    return client->quickSend<FkOnStartPrt>(molecule);
}

FkResult FkLayerEngine::onStop() {
    auto ret = FkEngine::onStop();
    FkAssert(ret == FK_OK, ret);
    ret = client->quickSend<FkOnStopPrt>(molecule);
    FkAssert(ret == FK_OK, ret);
    return renderEngine->stop();
}

FkResult FkLayerEngine::setSurface(std::shared_ptr<FkGraphicWindow> win) {
    auto msg = FkMessage::obtain(FK_MSG_SET_SURFACE);
    msg->sp = win;
    return sendMessage(msg);
}

FkResult FkLayerEngine::_setSurface(std::shared_ptr<FkMessage> msg) {
    FkAssert(nullptr != msg->sp, FK_NPE);
    auto proto = std::make_shared<FkSetSurfacePrt>();
    proto->win = std::dynamic_pointer_cast<FkGraphicWindow>(msg->sp);
    return client->with(molecule)->send(proto);
}

FkResult FkLayerEngine::notifyRender() {
    auto msg = FkMessage::obtain(FK_MSG_NOTIFY_RENDER);
    msg->flags = FkMessage::FLAG_UNIQUE;
    return sendMessage(msg);
}

FkResult FkLayerEngine::_notifyRender(std::shared_ptr<FkMessage> msg) {
    auto proto = std::make_shared<FkRenderRequestPrt>();
    proto->req = std::make_shared<FkRenderRequest>();
    return client->quickSend(proto, molecule);
}

FkID FkLayerEngine::newLayer(FkID expectId) {
    auto msg = FkMessage::obtain(FK_MSG_NEW_LAYER);
    msg->arg1 = expectId;
    msg->withPromise();
    auto ret = sendMessage(msg);
    FkID id = FK_ID_NONE;
    if (FK_OK == ret) {
        msg->getPromiseResult(id);
    }
    return id;
}

FkResult FkLayerEngine::_newLayer(std::shared_ptr<FkMessage> msg) {
    auto proto = std::make_shared<FkGraphicNewLayerPrt>();
    proto->expectId = msg->arg1;
    auto ret = client->with(molecule)->send(proto);
    msg->setPromiseResult(proto->layer ? proto->layer->id : FK_ID_NONE);
    return ret;
}

FkID FkLayerEngine::newLayerWithColor(FkSize size, FkColor color) {
    auto id = newLayer();
    if (FK_ID_NONE != id) {
        auto colorCom = std::make_shared<FkColorCompo>(color);
        auto sizeCom = std::make_shared<FkSizeCompo>(size);
        auto layer = std::make_shared<FkGraphicLayer>();
        layer->id = id;
        layer->addComponent(colorCom);
        layer->addComponent(sizeCom);
        auto msg = FkMessage::obtain(FK_MSG_UPDATE_LAYER_WITH_COLOR);
        msg->sp = layer;
        sendMessage(msg);
    }
    return id;
}

FkResult FkLayerEngine::_updateLayerWithColor(std::shared_ptr<FkMessage> msg) {
    auto layer = std::dynamic_pointer_cast<FkGraphicLayer>(msg->sp);
    auto prt = std::make_shared<FkGraphicUpdateLayerPrt>();
    prt->layer = layer;
    prt->scaleType = kScaleType::CENTER_INSIDE;
    auto sizeCom = prt->layer->findComponent<FkSizeCompo>();
    if (sizeCom) {
        setCanvasSizeInternal(sizeCom->size, true);
    }
    return client->with(molecule)->send(prt);
}

FkResult FkLayerEngine::setCanvasSize(FkSize size) {
    auto msg = FkMessage::obtain(FK_MSG_SET_CANVAS_SIZE);
    msg->sp = std::make_shared<FkSize>(size);
    return sendMessage(msg);
}

FkResult FkLayerEngine::_setCanvasSize(std::shared_ptr<FkMessage> msg) {
    FkAssert(nullptr != msg->sp, FK_NPE);
    auto size = std::dynamic_pointer_cast<FkSize>(msg->sp);
    return setCanvasSizeInternal(*size, false);
}

FkResult FkLayerEngine::setCanvasSizeInternal(FkSize &size, bool isInitialize) {
    auto queryProto = std::make_shared<FkQuerySizeProto>();
    auto ret = client->with(molecule)->send(queryProto);
    FkAssert(FK_OK == ret, FK_FAIL);
    if (isInitialize && !queryProto->value.isZero()) {
        return FK_FAIL;
    }
    if (size == queryProto->value) {
        return FK_FAIL;
    }

    auto sizeComp = std::make_shared<FkSizeCompo>(size);

    auto layer = std::make_shared<FkGraphicLayer>();
    layer->id = Fk_CANVAS_ID;
    layer->addComponent(sizeComp);
    auto updateProto = std::make_shared<FkGraphicUpdateLayerPrt>();
    updateProto->layer = layer;
    updateProto->scaleType = kScaleType::CENTER_INSIDE;
    return client->with(molecule)->send(updateProto);
}

FkResult FkLayerEngine::postTranslate(FkID layer, int32_t dx, int32_t dy) {
    auto msg = FkMessage::obtain(FK_MSG_POST_TRANSLATE);
    msg->arg1 = layer;
    msg->sp = std::make_shared<FkIntVec2>(dx, dy);
    return sendMessage(msg);
}

FkResult FkLayerEngine::_postTranslate(std::shared_ptr<FkMessage> msg) {
    auto measure = std::make_shared<FkMeasureTransProto>();
    measure->layerId = msg->arg1;
    measure->value = *std::dynamic_pointer_cast<FkIntVec2>(msg->sp);
    FkAssert(FK_OK == client->quickSend(measure, molecule), FK_FAIL);
    auto proto = std::make_shared<FkLayerPostTransProto>();
    proto->layer = measure->layerId;
    proto->value = measure->value;
    return client->quickSend(proto, molecule);
}

FkResult FkLayerEngine::postScale(FkID layer, float dx, float dy) {
    auto msg = FkMessage::obtain(FK_MSG_POST_SCALE);
    msg->arg1 = layer;
    msg->sp = std::make_shared<FkFloatVec3>(dx, dy, 1.0f);
    return sendMessage(msg);
}

FkResult FkLayerEngine::_postScale(std::shared_ptr<FkMessage> msg) {
    auto proto = std::make_shared<FkLayerPostScaleProto>();
    proto->layer = msg->arg1;
    proto->value = *Fk_POINTER_CAST(FkFloatVec3, msg->sp);
    return client->quickSend(proto, molecule);
}

FkResult FkLayerEngine::postRotation(FkID layer, FkRational &rational) {
    auto msg = FkMessage::obtain(FK_MSG_POST_ROTATION);
    msg->arg1 = layer;
    msg->sp = std::make_shared<FkRational>(rational);
    return sendMessage(msg);
}

FkResult FkLayerEngine::_postRotation(std::shared_ptr<FkMessage> msg) {
    auto proto = std::make_shared<FkLayerPostRotateProto>();
    proto->layer = msg->arg1;
    proto->value = *std::dynamic_pointer_cast<FkRational>(msg->sp);
    return client->quickSend(proto, molecule);
}

FkResult FkLayerEngine::drawPoint(FkID layer, FkColor color, int32_t x, int32_t y) {
    auto proto = std::make_shared<FkDrawPointProto>();
    proto->layer = layer;
    proto->color = color;
    proto->value.x = x;
    proto->value.y = y;
    auto msg = FkMessage::obtain(FK_MSG_DRAW_POINT);
    msg->sp = proto;
    return sendMessage(msg);
}

FkResult FkLayerEngine::_drawPoint(std::shared_ptr<FkMessage> msg) {
    auto proto = Fk_POINTER_CAST(FkDrawPointProto, msg->sp);
    return client->quickSend(proto, molecule);
}

FkResult FkLayerEngine::queryLayers(std::vector<std::shared_ptr<FkGraphicLayer>> &vec) {
    auto msg = FkMessage::obtain(FK_MSG_QUERY_LAYERS);
    msg->withPromise();
    auto ret = sendMessage(msg);
    if (FK_OK == ret && FK_OK == msg->getPromiseResult(vec)) {
        return FK_OK;
    }
    return ret;
}

FkResult FkLayerEngine::_queryLayers(std::shared_ptr<FkMessage> msg) {
    auto proto = std::make_shared<FkQueryLayersProto>();
    auto ret = client->with(molecule)->send(proto);
    msg->setPromiseResult(proto->layers);
    return ret;
}
