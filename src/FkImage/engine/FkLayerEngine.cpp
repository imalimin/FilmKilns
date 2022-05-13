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
#include "FkMeasurePointProto.h"
#include "FkDrawPointProto.h"
#include "FkQueryLayersProto.h"
#include "FkRenderEngineCompo.h"
#include "FkLayerSetTransProto.h"
#include "FkLayerSetRotateProto.h"
#include "FkLayerSetScaleProto.h"
#include "FkCropProto.h"
#include "FkMath.h"
#include "FkQueryWinSizeProto.h"
#include "FkRemoveLayerProto.h"
#include "FkScaleTypeProto.h"
#include "FkReadPixelsProto.h"
#include "FkDrawPathProto.h"

FK_IMPL_CLASS_TYPE(FkLayerEngine, FkEngine)

const FkID FkLayerEngine::MSG_NOTIFY_RENDER = 0x100;

FkLayerEngine::FkLayerEngine(std::shared_ptr<FkEngine> &renderEngine, std::string name)
        : FkEngine(name), renderEngine(renderEngine) {

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

FkResult FkLayerEngine::setSurface(std::shared_ptr<FkGraphicWindow> win, int32_t scaleType) {
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_setSurface));
    msg->sp = win;
    msg->arg1 = scaleType;
    return sendMessage(msg);
}

FkResult FkLayerEngine::_setSurface(std::shared_ptr<FkMessage> msg) {
    FkAssert(nullptr != msg->sp, FK_NPE);
    auto proto = std::make_shared<FkSetSurfacePrt>();
    proto->win = std::dynamic_pointer_cast<FkGraphicWindow>(msg->sp);
    auto ret = client->with(molecule)->send(proto);

    auto type = static_cast<kScaleType>(msg->arg1);
    auto scaleProto = std::make_shared<FkScaleTypeProto>(Fk_CANVAS_ID, type);
    client->with(molecule)->send(scaleProto);
    return ret;
}

FkResult FkLayerEngine::notifyRender() {
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_notifyRender));
    msg->what = MSG_NOTIFY_RENDER;
    msg->flags = FkMessage::FLAG_UNIQUE;
    return sendMessage(msg);
}

FkResult FkLayerEngine::_notifyRender(std::shared_ptr<FkMessage> msg) {
    auto proto = std::make_shared<FkRenderRequestPrt>();
    proto->req = std::make_shared<FkRenderRequest>();
    return client->quickSend(proto, molecule);
}

FkID FkLayerEngine::newLayer(FkID expectId) {
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_newLayer));
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

FkID FkLayerEngine::newLayerWithColor(FkSize size, FkColor color, FkID expectId) {
    auto id = newLayer(expectId);
    if (FK_ID_NONE != id) {
        auto colorCom = std::make_shared<FkColorCompo>(color);
        auto sizeCom = std::make_shared<FkSizeCompo>(size);
        auto layer = std::make_shared<FkGraphicLayer>();
        layer->id = id;
        layer->addComponent(colorCom);
        layer->addComponent(sizeCom);
        auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_updateLayerWithColor));
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
    auto sizeCom = FK_FIND_COMPO(prt->layer, FkSizeCompo);
    if (sizeCom) {
        setCanvasSizeInternal(sizeCom->size, true);
    }
    return client->with(molecule)->send(prt);
}

FkResult FkLayerEngine::removeLayer(FkID layer) {
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_removeLayer));
    msg->arg1 = layer;
    return sendMessage(msg);
}

FkResult FkLayerEngine::_removeLayer(std::shared_ptr<FkMessage> msg) {
    auto proto = std::make_shared<FkRemoveLayerProto>(msg->arg1);
    return client->with(molecule)->send(proto);
}

FkResult FkLayerEngine::setCanvasSize(FkSize size) {
    FkAssert(!size.isZero(), FK_FAIL);
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_setCanvasSize));
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
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_postTranslate));
    msg->arg1 = layer;
    msg->sp = std::make_shared<FkIntVec2>(dx, dy);
    return sendMessage(msg);
}

FkResult FkLayerEngine::_postTranslate(std::shared_ptr<FkMessage> msg) {
    auto measure = std::make_shared<FkMeasureTransProto>();
    measure->layerId = msg->arg1;
    measure->value = *std::dynamic_pointer_cast<FkIntVec2>(msg->sp);
    auto ret = client->quickSend(measure, molecule);
    FkAssert(FK_OK == ret, FK_FAIL);
    auto proto = std::make_shared<FkLayerPostTransProto>();
    proto->layer = measure->layerId;
    proto->value = measure->value;
    return client->quickSend(proto, molecule);
}

FkResult FkLayerEngine::setTranslate(FkID layer, int32_t x, int32_t y) {
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_setTranslate));
    msg->arg1 = layer;
    msg->sp = std::make_shared<FkIntVec2>(x, y);
    return sendMessage(msg);
}

FkResult FkLayerEngine::_setTranslate(std::shared_ptr<FkMessage> msg) {
    auto vec = std::dynamic_pointer_cast<FkIntVec2>(msg->sp);
    auto proto = std::make_shared<FkLayerSetTransProto>();
    proto->layer = msg->arg1;
    proto->value = *vec;
    return client->quickSend(proto, molecule);
}

FkResult FkLayerEngine::postScale(FkID layer, float dx, float dy) {
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_postScale));
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

FkResult FkLayerEngine::setScale(FkID layer, float x, float y) {
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_setScale));
    msg->arg1 = layer;
    msg->sp = std::make_shared<FkFloatVec3>(x, y, 1.0f);
    return sendMessage(msg);
}

FkResult FkLayerEngine::_setScale(std::shared_ptr<FkMessage> msg) {
    auto vec = std::dynamic_pointer_cast<FkFloatVec3>(msg->sp);
    auto proto = std::make_shared<FkLayerSetScaleProto>();
    proto->layer = msg->arg1;
    proto->value = *vec;
    return client->quickSend(proto, molecule);
}

FkResult FkLayerEngine::postRotation(FkID layer, FkRational &rational) {
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_postRotation));
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

FkResult FkLayerEngine::setRotation(FkID layer, FkRational &rational) {
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_setRotation));
    msg->arg1 = layer;
    msg->sp = std::make_shared<FkRational>(rational);
    return sendMessage(msg);
}

FkResult FkLayerEngine::_setRotation(std::shared_ptr<FkMessage> msg) {
    auto value = std::dynamic_pointer_cast<FkRational>(msg->sp);
    auto proto = std::make_shared<FkLayerSetRotateProto>();
    proto->layer = msg->arg1;
    proto->value = *value;
    return client->quickSend(proto, molecule);
}

FkResult FkLayerEngine::drawPoint(FkID layer, FkColor color, int32_t size, int32_t x, int32_t y) {
    auto proto = std::make_shared<FkDrawPointProto>();
    proto->layer = layer;
    proto->color = color;
    proto->value.x = x;
    proto->value.y = y;
    proto->size = size;
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_drawPoint));
    msg->sp = proto;
    return sendMessage(msg);
}

FkResult FkLayerEngine::_drawPoint(std::shared_ptr<FkMessage> msg) {
    auto proto = Fk_POINTER_CAST(FkDrawPointProto, msg->sp);
    auto measureProto = std::make_shared<FkMeasurePointProto>();
    measureProto->layerId = proto->layer;
    measureProto->value = proto->value;
    auto ret = client->with(molecule)->send(measureProto);
    FkAssert(FK_OK == ret, FK_FAIL);
    //  -,- |
    //------0------->
    //      | +,+
    proto->value = measureProto->value;
    return client->with(molecule)->send(proto);
}

FkResult FkLayerEngine::queryLayers(std::vector<std::shared_ptr<FkGraphicLayer>> &vec) {
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_queryLayers));
    msg->withPromise();
    auto ret = sendMessage(msg);
    if (FK_OK == ret && FK_OK == msg->getPromiseResult(vec)) {
        return FK_OK;
    }
    return ret;
}

FkResult FkLayerEngine::_queryLayers(std::shared_ptr<FkMessage> msg) {
    msg->setPromiseResult(queryLayers());
    return FK_OK;
}

std::vector<std::shared_ptr<FkGraphicLayer>> FkLayerEngine::queryLayers() {
    auto proto = std::make_shared<FkQueryLayersProto>();
    auto ret = client->with(molecule)->send(proto);
//    FkAssert(FK_OK != ret, proto->layers);
    return proto->layers;
}

FkResult FkLayerEngine::crop(FkIntRect &rect) {
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_crop));
    msg->sp = std::make_shared<FkIntRect>(rect);
    return sendMessage(msg);
}

FkResult FkLayerEngine::_crop(std::shared_ptr<FkMessage> &msg) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(rect, FkIntRect, msg->sp);
    auto queryProto = std::make_shared<FkQueryWinSizeProto>();
    auto ret = client->with(molecule)->send(queryProto);
    auto deltaX = (queryProto->winSize.getWidth() / 2 - (rect->right() + rect->left()) / 2);
    auto deltaY = (queryProto->winSize.getHeight() / 2 - (rect->bottom() + rect->top()) / 2);

    auto layers = queryLayers();
    for (auto &it : layers) {
        if (it->id != Fk_CANVAS_ID) {
            auto msg0 = FkMessage::obtain(0);
            msg0->arg1 = it->id;
            msg0->sp = std::make_shared<FkIntVec2>(deltaX, deltaY);
            _postTranslate(msg0);
        }
    }

    std::vector<FkIntVec2> vec;
    vec.emplace_back(FkIntVec2(rect->left(), rect->top()));
    vec.emplace_back(FkIntVec2(rect->right(), rect->top()));
    vec.emplace_back(FkIntVec2(rect->right(), rect->bottom()));
    vec.emplace_back(FkIntVec2(rect->left(), rect->bottom()));
    for (auto &it : vec) {
        auto measureProto = std::make_shared<FkMeasurePointProto>();
        measureProto->layerId = Fk_CANVAS_ID;
        measureProto->value = it;
        ret = client->with(molecule)->send(measureProto);
        FkAssert(FK_OK == ret, FK_FAIL);
        it = measureProto->value;
    }
    auto leftTop = vec[0];
    auto rightTop = vec[1];
    auto rightBottom = vec[2];
    auto leftBottom = vec[3];
    FkSize size(FkMath::distance(leftTop, rightTop),
                FkMath::distance(leftTop, leftBottom));
    return setCanvasSizeInternal(size, false);
}

FkResult FkLayerEngine::cropLayer(FkID layerId, FkIntRect &rect) {
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_cropLayer));
    msg->arg1 = layerId;
    msg->sp = std::make_shared<FkIntRect>(rect);
    return sendMessage(msg);
}

FkResult FkLayerEngine::_cropLayer(std::shared_ptr<FkMessage> &msg) {
    return 0;
}

FkResult FkLayerEngine::readPixels(FkID layerId, FkIntVec2 &pos, FkSize &size,
                                   std::function<void(std::shared_ptr<FkBuffer>, FkSize)> finishCallback) {
    auto proto = std::make_shared<FkReadPixelsProto>();
    proto->layerId = layerId;
    proto->pos = pos;
    proto->size = size;
    proto->finishCallback = std::move(finishCallback);

    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_readPixels));
    msg->arg1 = layerId;
    msg->sp = proto;
    return sendMessage(msg);
}

FkResult FkLayerEngine::_readPixels(std::shared_ptr<FkMessage> &msg) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkReadPixelsProto, msg->sp);
    return client->with(molecule)->send(proto);
}

FkResult FkLayerEngine::drawPath(FkID layerId,
                                 int32_t x, int32_t y,
                                 std::shared_ptr<FkPaint> &paint) {
    FkAssert(paint != nullptr && paint->strokeWidth > 0, FK_INVALID_DATA);
    FkIntVec2 point(x, y);
    auto proto = std::make_shared<FkDrawPathProto>(layerId, point);
    proto->isFinish = false;
    proto->paint = paint;

    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_drawPath));
    msg->sp = proto;
    return sendMessage(msg);
}

FkResult FkLayerEngine::_drawPath(std::shared_ptr<FkMessage> &msg) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkDrawPathProto, msg->sp);
    auto measureProto = std::make_shared<FkMeasurePointProto>();
    measureProto->layerId = proto->layerId;
    measureProto->value = proto->point;
    auto ret = client->with(molecule)->send(measureProto);
    FkAssert(FK_OK == ret, FK_FAIL);

    proto->point = measureProto->value;
    return client->with(molecule)->send(proto);
}

FkResult FkLayerEngine::drawPathFinish(FkID layerId) {
    FkIntVec2 point(0, 0);
    auto proto = std::make_shared<FkDrawPathProto>(layerId, point);
    proto->isFinish = true;

    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_drawPath));
    msg->sp = proto;
    return sendMessage(msg);
}