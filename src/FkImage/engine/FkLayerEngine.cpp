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
#include "FkGraphicNewTexPtl.h"
#include "FkSetSurfacePrt.h"
#include "FkRenderRequestPrt.h"
#include "FkGraphicUpdateTexPrt.h"
#include "FkGraphicTexDelPtl.h"
#include "FkSetSizeProto.h"
#include "FkQuerySizeProto.h"
#include "FkIntVec2.h"
#include "FkFloatVec3.h"
#include "FkLayerPostTransProto.h"
#include "FkLayerPostScaleProto.h"
#include "FkLayerPostRotateProto.h"
#include "FkMeasureTransProto.h"
#include "FkDrawPointProto.h"
#include "FkRenderEngineCompo.h"

const FkID FkLayerEngine::FK_MSG_NEW_LAYER = FK_KID('F', 'K', 'E', 0x10);
const FkID FkLayerEngine::FK_MSG_UPDATE_LAYER_WITH_COLOR = FK_KID('F', 'K', 'E', 0x11);
const FkID FkLayerEngine::FK_MSG_SET_SURFACE = FK_KID('F', 'K', 'E', 0x12);
const FkID FkLayerEngine::FK_MSG_NOTIFY_RENDER = FK_KID('F', 'K', 'E', 0x13);
const FkID FkLayerEngine::FK_MSG_SET_CANVAS_SIZE = FK_KID('F', 'K', 'E', 0x14);
const FkID FkLayerEngine::FK_MSG_POST_TRANSLATE = FK_KID('F', 'K', 'E', 0x15);
const FkID FkLayerEngine::FK_MSG_POST_SCALE = FK_KID('F', 'K', 'E', 0x16);
const FkID FkLayerEngine::FK_MSG_POST_ROTATION = FK_KID('F', 'K', 'E', 0x17);
const FkID FkLayerEngine::FK_MSG_DRAW_POINT = FK_KID('F', 'K', 'E', 0x18);

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
    ret = renderEngine->destroy();
    FkAssert(ret == FK_OK, ret);
    return client->quickSend<FkOnDestroyPrt>(molecule);
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
    ret = renderEngine->stop();
    FkAssert(ret == FK_OK, ret);
    return client->quickSend<FkOnStopPrt>(molecule);
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
    return sendMessage(msg);;
}

FkResult FkLayerEngine::_notifyRender(std::shared_ptr<FkMessage> msg) {
    auto proto = std::make_shared<FkRenderRequestPrt>();
    proto->req = std::make_shared<FkRenderRequest>();
    return client->quickSend(proto, molecule);
}

FkID FkLayerEngine::newLayer() {
    auto msg = FkMessage::obtain(FK_MSG_NEW_LAYER);
    msg->promise = std::make_shared<std::promise<std::shared_ptr<FkObject>>>();
    sendMessage(msg);
    auto result = std::static_pointer_cast<FkGraphicNewLayerPrt>(msg->promise->get_future().get());
    if (result->layer) {
        return result->layer->id;
    }
    return FK_ID_NONE;
}

FkResult FkLayerEngine::_newLayer(std::shared_ptr<FkMessage> msg) {
    auto prt = std::make_shared<FkGraphicNewLayerPrt>();
    auto ret = client->quickSend<FkGraphicNewLayerPrt>(prt, molecule);
    if (nullptr != msg->promise) {
        msg->promise->set_value(prt);
    }
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
//    msg->flags = FkMessage::FLAG_UNIQUE;
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
    msg->flags = FkMessage::FLAG_UNIQUE;
    return sendMessage(msg);
}

FkResult FkLayerEngine::_postRotation(std::shared_ptr<FkMessage> msg) {
    auto proto = std::make_shared<FkLayerPostRotateProto>();
    proto->layer = msg->arg1;
    proto->value = *Fk_POINTER_CAST(FkRational, msg->sp);
    return client->quickSend(proto, molecule);
}

FkResult FkLayerEngine::drawPoint(FkID layer, FkColor color, int32_t x, int32_t y) {
    auto comp = std::make_shared<FkDrawPointProto>();
    comp->layer = layer;
    comp->color = color;
    comp->value.x = x;
    comp->value.y = y;
    auto msg = FkMessage::obtain(FK_MSG_DRAW_POINT);
    msg->sp = comp;
    return sendMessage(msg);
}

FkResult FkLayerEngine::_drawPoint(std::shared_ptr<FkMessage> msg) {
    auto proto = Fk_POINTER_CAST(FkDrawPointProto, msg->sp);
    return client->quickSend(proto, molecule);
}
