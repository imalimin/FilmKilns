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
#include "FkUpdateLayerModelProto.h"
#include "FkImageContext.h"
#include "FkRotateComponent.h"
#include "FkDeviceImageCompo.h"
#include "FkShadowLayerCompo.h"
#include "FkRenderCanvasTexCallbackProto.h"
#include "FkLayerSetVisibilityProto.h"
#include "FkLayerCopyProto.h"

const FkID FkLayerEngine::MSG_NOTIFY_RENDER = 0x100;

FK_IMPL_CLASS_TYPE(FkLayerEngine, FkEngine)

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
    proto->context = std::make_shared<FkImageContext>();
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

FkResult FkLayerEngine::notifyRender(int64_t timestamp) {
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_notifyRender));
    msg->what = MSG_NOTIFY_RENDER;
    msg->flags = FkMessage::FLAG_UNIQUE;
    msg->arg2 = timestamp;
    return sendMessage(msg);
}

FkResult FkLayerEngine::_notifyRender(std::shared_ptr<FkMessage> msg) {
    auto proto = std::make_shared<FkRenderRequestPrt>();
    proto->req = std::make_shared<FkRenderRequest>();
    proto->timestamp = msg->arg2;
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
        auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_updateLayer));
        msg->sp = layer;
        sendMessage(msg);
    }
    return id;
}

FkResult FkLayerEngine::_updateLayer(std::shared_ptr<FkMessage> &msg) {
    auto layer = std::dynamic_pointer_cast<FkGraphicLayer>(msg->sp);
    auto proto = std::make_shared<FkGraphicUpdateLayerPrt>();
    proto->layer = layer;
    proto->scaleType = kScaleType::CENTER_INSIDE;
    auto sizeCompo = FK_FIND_COMPO(proto->layer, FkSizeCompo);
    if (sizeCompo) {
        setCanvasSizeInternal(sizeCompo->size, true);
    }
    return client->with(molecule)->send(proto);
}

FkID FkLayerEngine::newLayerWithDeviceImage(std::shared_ptr<FkDeviceImage> deviceImage,
                                            FkSize size, FkID expectId) {
    auto id = newLayer(expectId);
    if (FK_ID_NONE != id) {
        auto deviceImageCompo = std::make_shared<FkDeviceImageCompo>(deviceImage);
        auto sizeCom = std::make_shared<FkSizeCompo>(size);
        auto layer = std::make_shared<FkGraphicLayer>();
        layer->id = id;
        layer->addComponent(deviceImageCompo);
        layer->addComponent(sizeCom);
        auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_updateLayer));
        msg->sp = layer;
        sendMessage(msg);
    }
    return id;
}

FkID FkLayerEngine::newShadowLayer(FkID parentLayerId, FkSize size) {
    auto id = newLayer();
    if (FK_ID_NONE != id) {
        auto colorCompo = std::make_shared<FkColorCompo>(FkColor::transparent());
        auto sizeCompo = std::make_shared<FkSizeCompo>(size);
        auto shadowLayerCompo = std::make_shared<FkShadowLayerCompo>(parentLayerId);
        auto layer = std::make_shared<FkGraphicLayer>();
        layer->id = id;
        layer->addComponent(colorCompo);
        layer->addComponent(sizeCompo);
        layer->addComponent(shadowLayerCompo);
        auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_updateLayer));
        msg->sp = layer;
        sendMessage(msg);
    }
    return id;

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

FkResult FkLayerEngine::clearLayer(FkID layerId) {
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_clearLayer));
    msg->arg1 = layerId;
    return sendMessage(msg);
}

FkResult FkLayerEngine::_clearLayer(std::shared_ptr<FkMessage> &msg) {
    auto layer = std::make_shared<FkGraphicLayer>();
    layer->id = msg->arg1;
    layer->addComponent(std::make_shared<FkColorCompo>(FkColor::transparent()));
    auto proto = std::make_shared<FkGraphicUpdateLayerPrt>();
    proto->layer = layer;
    proto->scaleType = kScaleType::CENTER_INSIDE;
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
    auto point = std::dynamic_pointer_cast<FkIntVec2>(msg->sp);
    auto measure = std::make_shared<FkMeasureTransProto>();
    measure->layerId = msg->arg1;
    measure->value = FkDoubleVec2(point->x, point->y);
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
    auto point = std::dynamic_pointer_cast<FkIntVec2>(msg->sp);
    auto proto = std::make_shared<FkLayerSetTransProto>();
    proto->layer = msg->arg1;
    proto->value = FkDoubleVec2(point->x, point->y);;
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
    FkAssert(msg->sp != nullptr, FK_FAIL);
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
    measureProto->value = FkDoubleVec2(proto->value.x, proto->value.y);
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

    std::shared_ptr<FkGraphicLayer> canvas = nullptr;
    auto layers = queryLayers();
    for (auto &it : layers) {
        if (it->id == Fk_CANVAS_ID) {
            canvas = it;
            continue;
        }
        auto msg0 = FkMessage::obtain(0);
        msg0->arg1 = it->id;
        msg0->sp = std::make_shared<FkIntVec2>(deltaX, deltaY);
        _postTranslate(msg0);
    }

    std::vector<FkDoubleVec2> vec;
    vec.emplace_back(FkDoubleVec2(rect->left(), rect->top()));
    vec.emplace_back(FkDoubleVec2(rect->right(), rect->top()));
    vec.emplace_back(FkDoubleVec2(rect->right(), rect->bottom()));
    vec.emplace_back(FkDoubleVec2(rect->left(), rect->bottom()));
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
    ret = setCanvasSizeInternal(size, false);
    FkRational canvasRotation(0, 1);
    if (FK_OK == ret && canvas) {
        auto rotateCompo = FK_FIND_COMPO(canvas, FkRotateComponent);
        canvasRotation = rotateCompo->value;
    }
    if (FK_OK == ret) {
        auto msg0 = FkMessage::obtain(0);
        msg0->arg1 = Fk_CANVAS_ID;
        msg0->sp = std::make_shared<FkRational>(0, 1);
        ret = _setRotation(msg0);
    }
    if (FK_OK == ret && canvasRotation.num != 0) {
        for (auto &it: layers) {
            if (it->id == Fk_CANVAS_ID) {
                continue;
            }
            auto msg0 = FkMessage::obtain(0);
            msg0->arg1 = it->id;
            msg0->sp = std::make_shared<FkRational>(canvasRotation);
            _postRotation(msg0);
        }
    }
    return ret;
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
    FkDoubleVec2 point(x, y);
    auto proto = std::make_shared<FkDrawPathProto>(layerId, point);
    proto->isActionFinish = false;
    proto->paint = paint;

    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_drawPath));
    msg->sp = proto;
    return sendMessage(msg);
}

FkResult FkLayerEngine::_drawPath(std::shared_ptr<FkMessage> &msg) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkDrawPathProto, msg->sp);
    auto measureProto = std::make_shared<FkMeasurePointProto>();
    measureProto->layerId = proto->layerId;
    measureProto->value = FkDoubleVec2(proto->point.x, proto->point.y);
    auto ret = client->with(molecule)->send(measureProto);
    FkAssert(FK_OK == ret, FK_FAIL);

    proto->point = measureProto->value;
    return client->with(molecule)->send(proto);
}

FkResult FkLayerEngine::drawPathFinish(FkID layerId) {
    FkDoubleVec2 point(0, 0);
    auto proto = std::make_shared<FkDrawPathProto>(layerId, point);
    proto->isActionFinish = true;

    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_drawPath));
    msg->sp = proto;
    return sendMessage(msg);
}

FkResult FkLayerEngine::clearPaths(FkID layerId) {
    FkDoubleVec2 point(0, 0);
    auto proto = std::make_shared<FkDrawPathProto>(layerId, point);
    proto->isActionClear = true;

    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_drawPath));
    msg->sp = proto;
    return sendMessage(msg);
}

FkResult FkLayerEngine::updateLayerWithModel(FkID layerId,
                                             std::shared_ptr<FkModelInterface> &modelInterface) {
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_updateLayerWithModel));
    msg->arg1 = layerId;
    msg->sp = modelInterface;
    return sendMessage(msg);
}

FkResult FkLayerEngine::_updateLayerWithModel(std::shared_ptr<FkMessage> &msg) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(modelInterface, FkModelInterface, msg->sp);
    auto proto = std::make_shared<FkUpdateLayerModelProto>(modelInterface);
    return client->with(molecule)->send(proto);
}

FkResult FkLayerEngine::setRenderCanvasTexCallback(std::function<void(uint32_t, FkSize, int64_t)> func) {
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_setRenderCanvasTexCallback));
    msg->sp = std::make_shared<FkRenderCanvasTexCallbackProto>(func);
    return sendMessage(msg);
}

FkResult FkLayerEngine::_setRenderCanvasTexCallback(std::shared_ptr<FkMessage> &msg) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkRenderCanvasTexCallbackProto, msg->sp);
    return client->with(molecule)->send(proto);
}

FkResult FkLayerEngine::setLayerVisibility(FkID layerId, int visibility) {
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_setLayerVisibility));
    msg->arg1 = layerId;
    msg->arg2 = visibility;
    return sendMessage(msg);
}

FkResult FkLayerEngine::_setLayerVisibility(std::shared_ptr<FkMessage> &msg) {
    auto proto = std::make_shared<FkLayerSetVisibilityProto>();
    proto->layerId = msg->arg1;
    proto->visibility = msg->arg2;
    return client->with(molecule)->send(proto);
}

FkResult FkLayerEngine::copyLayer(FkID srcLayerId, FkID dstLayerId) {
    FkAssert(srcLayerId != dstLayerId, FK_NOT_SUPPORT);
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_copyLayer));
    msg->arg1 = srcLayerId;
    msg->arg2 = dstLayerId;
    return sendMessage(msg);
}

FkResult FkLayerEngine::_copyLayer(std::shared_ptr<FkMessage> &msg) {
    auto proto = std::make_shared<FkLayerCopyProto>(msg->arg1, msg->arg2);
    return client->with(molecule)->send(proto);
}