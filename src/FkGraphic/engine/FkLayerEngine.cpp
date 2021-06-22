/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkLayerEngine.h"
#include "FkGraphicNewLayerPrt.h"
#include "FkGraphicUpdateLayerPrt.h"
#include "FkColorComponent.h"
#include "FkSizeComponent.h"
#include "FkTexComponent.h"
#include "FkGraphicNewTexPtl.h"
#include "FkSetSurfacePrt.h"
#include "FkRenderRequestPrt.h"
#include "FkGraphicUpdateTexPrt.h"
#include "FkGraphicTexDelPtl.h"

const FkID FkLayerEngine::FK_MSG_NEW_LAYER = FK_KID('F', 'K', 'E', 0x10);
const FkID FkLayerEngine::FK_MSG_UPDATE_LAYER_WITH_COLOR = FK_KID('F', 'K', 'E', 0x11);
const FkID FkLayerEngine::FK_MSG_SET_SURFACE = FK_KID('F', 'K', 'E', 0x12);
const FkID FkLayerEngine::FK_MSG_NOTIFY_RENDER = FK_KID('F', 'K', 'E', 0x13);

FkLayerEngine::FkLayerEngine(std::string name) : FkEngine(std::move(name)) {
    FK_MARK_SUPER
    FK_REG_MSG(FK_MSG_NEW_LAYER, FkLayerEngine::_newLayer);
    FK_REG_MSG(FK_MSG_UPDATE_LAYER_WITH_COLOR, FkLayerEngine::_updateLayerWithColor);
    FK_REG_MSG(FK_MSG_SET_SURFACE, FkLayerEngine::_setSurface);
    FK_REG_MSG(FK_MSG_NOTIFY_RENDER, FkLayerEngine::_notifyRender);
    client = std::make_shared<FkLocalClient>();
    molecule = std::make_shared<FkGraphicMolecule>();
}

FkLayerEngine::~FkLayerEngine() {

}

FkResult FkLayerEngine::onCreate() {
    auto ret = FkEngine::onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    client->quickSend<FkOnCreatePrt>(molecule);
    return ret;
}

FkResult FkLayerEngine::onDestroy() {
    auto ret = FkEngine::onDestroy();
    if (FK_OK != ret) {
        return ret;
    }
    client->quickSend<FkOnDestroyPrt>(molecule);
    return ret;
}

FkResult FkLayerEngine::onStart() {
    auto ret = FkEngine::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    client->quickSend<FkOnStartPrt>(molecule);
    return ret;
}

FkResult FkLayerEngine::onStop() {
    auto ret = FkEngine::onStop();
    if (FK_OK != ret) {
        return ret;
    }
    client->quickSend<FkOnStopPrt>(molecule);
    return ret;
}

FkResult FkLayerEngine::notifyRender() {
    auto msg = FkMessage::obtain(FK_MSG_NOTIFY_RENDER);
    return sendMessage(msg);;
}

FkResult FkLayerEngine::setSurface(std::shared_ptr<FkGraphicWindow> win) {
    auto msg = FkMessage::obtain(FK_MSG_SET_SURFACE);
    msg->sp = std::move(win);
    return sendMessage(msg);;
}

FkID FkLayerEngine::newLayer() {
    auto msg = FkMessage::obtain(FK_MSG_NEW_LAYER);
    msg->promise = std::make_shared<std::promise<std::shared_ptr<FkObject>>>();
    sendMessage(msg);
    std::shared_ptr<FkGraphicNewLayerPrt> result = std::static_pointer_cast<FkGraphicNewLayerPrt>(
            msg->promise->get_future().get());
    if (result->layer) {
        return result->layer->id;
    }
    return FK_ID_NONE;
}

FkID FkLayerEngine::newLayerWithColor(FkSize size, FkColor color) {
    auto id = newLayer();
    if (FK_ID_NONE != id) {
        auto colorCom = std::make_shared<FkColorComponent>();
        colorCom->color = color;
        auto sizeCom = std::make_shared<FkSizeComponent>();
        sizeCom->size = size;
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

FkResult FkLayerEngine::_newLayer(std::shared_ptr<FkMessage> msg) {
    auto prt = std::make_shared<FkGraphicNewLayerPrt>();
    auto ret = client->quickSend<FkGraphicNewLayerPrt>(prt, molecule);
    if (nullptr != msg->promise) {
        msg->promise->set_value(prt);
    }
    return ret;
}

FkResult FkLayerEngine::_updateLayerWithColor(std::shared_ptr<FkMessage> msg) {
    auto texPrt = std::make_shared<FkGraphicNewTexPtl>();
    texPrt->fmt = FkColor::kFormat::RGBA;
    auto ret = client->quickSend<FkGraphicNewTexPtl>(texPrt, molecule);
    if (FK_OK == ret) {
        auto layer = Fk_POINTER_CAST(FkGraphicLayer, msg->sp);
        auto updateTexPrt = std::make_shared<FkGraphicUpdateTexPrt>();
        std::vector<std::shared_ptr<FkGraphicComponent>> vec;
        if (FK_OK == layer->findComponent(vec, FkClassType::type<FkSizeComponent>())) {
            updateTexPrt->size = Fk_POINTER_CAST(FkSizeComponent, vec[0])->size;
        } else {
            auto delPrt = std::make_shared<FkGraphicTexDelPtl>();
            delPrt->id = texPrt->id;
            client->quickSend<FkGraphicTexDelPtl>(delPrt, molecule);
            return FK_FAIL;
        }
        updateTexPrt->id = texPrt->id;

        ret = client->quickSend<FkGraphicUpdateTexPrt>(updateTexPrt, molecule);
        if (FK_OK != ret) {
            auto delPrt = std::make_shared<FkGraphicTexDelPtl>();
            delPrt->id = texPrt->id;
            client->quickSend<FkGraphicTexDelPtl>(delPrt, molecule);
            return ret;
        }
        auto com = std::make_shared<FkTexComponent>();
        com->id = texPrt->id;

        auto prt = std::make_shared<FkGraphicUpdateLayerPrt>();
        prt->layer = layer;
        prt->layer->addComponent(com);
        return client->quickSend<FkGraphicUpdateLayerPrt>(prt, molecule);
    }
    return ret;
}

FkResult FkLayerEngine::_setSurface(std::shared_ptr<FkMessage> msg) {
    auto prt = std::make_shared<FkSetSurfacePrt>();
    prt->win = std::static_pointer_cast<FkGraphicWindow>(msg->sp);
    return client->quickSend<FkSetSurfacePrt>(prt, molecule);
}

FkResult FkLayerEngine::_notifyRender(std::shared_ptr<FkMessage> msg) {
    auto prt = std::make_shared<FkRenderRequestPrt>();
    return client->quickSend<FkRenderRequestPrt>(prt, molecule);
}
