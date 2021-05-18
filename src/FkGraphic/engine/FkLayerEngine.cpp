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
#include "FkTexComponent.h"
#include "FkGraphicNewTexPtl.h"

const FkID FkLayerEngine::FK_MSG_NEW_LAYER = FK_KID('F', 'K', 'E', 0x10);
const FkID FkLayerEngine::FK_MSG_UPDATE_LAYER_WITH_FILE = FK_KID('F', 'K', 'E', 0x11);
const FkID FkLayerEngine::FK_MSG_UPDATE_LAYER_WITH_COLOR = FK_KID('F', 'K', 'E', 0x12);

FkLayerEngine::FkLayerEngine(std::string name) : FkEngine(std::move(name)) {
    FK_REG_MSG(FK_MSG_NEW_LAYER, FkLayerEngine::_newLayer)
    FK_REG_MSG(FK_MSG_UPDATE_LAYER_WITH_FILE, FkLayerEngine::_updateLayerWithFile)
    FK_REG_MSG(FK_MSG_UPDATE_LAYER_WITH_COLOR, FkLayerEngine::_updateLayerWithColor)
    client = std::make_shared<FkLocalClient>();
    molecule = std::make_shared<FkGraphicMolecule>();
}

FkLayerEngine::~FkLayerEngine() {

}

FkResult FkLayerEngine::create() {
    auto ret = FkEngine::create();
    if (FK_OK != ret) {
        return ret;
    }
    client->quickSend<FkOnCreatePrt>(molecule);
    return ret;
}

FkResult FkLayerEngine::destroy() {
    auto ret = FkEngine::destroy();
    if (FK_OK != ret) {
        return ret;
    }
    client->quickSend<FkOnDestroyPrt>(molecule);
    return ret;
}

FkResult FkLayerEngine::start() {
    auto ret = FkEngine::start();
    if (FK_OK != ret) {
        return ret;
    }
    client->quickSend<FkOnStartPrt>(molecule);
    return ret;
}

FkResult FkLayerEngine::stop() {
    auto ret = FkEngine::stop();
    if (FK_OK != ret) {
        return ret;
    }
    client->quickSend<FkOnStopPrt>(molecule);
    return ret;
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

FkID FkLayerEngine::newLayerWithFile(std::string path) {
    auto id = newLayer();
    if (FK_ID_NONE != id) {
        auto msg = FkMessage::obtain(FK_MSG_UPDATE_LAYER_WITH_COLOR);
        msg->arg3 = std::move(path);
        sendMessage(msg);
    }
    return id;
}

FkID FkLayerEngine::newLayerWithColor(FkSize size, FkColor color) {
    auto id = newLayer();
    if (FK_ID_NONE != id) {
        auto component = std::make_shared<FkColorComponent>();
        component->size = size;
        component->color = color;
        auto layer = std::make_shared<FkGraphicLayer>();
        layer->id = id;
        layer->addComponent(component);
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

FkResult FkLayerEngine::_updateLayerWithFile(std::shared_ptr<FkMessage> msg) {
    return FK_FAIL;
}

FkResult FkLayerEngine::_updateLayerWithColor(std::shared_ptr<FkMessage> msg) {
    auto texPrt = std::make_shared<FkGraphicNewTexPtl>();
    auto ret = client->quickSend<FkGraphicNewTexPtl>(texPrt, molecule);
    auto prt = std::make_shared<FkGraphicUpdateLayerPrt>();
    prt->layer = std::static_pointer_cast<FkGraphicLayer>(msg->sp);
    if (FK_OK == ret) {
        auto com = std::make_shared<FkTexComponent>();
        com->texId = texPrt->texId;
        prt->layer->addComponent(com);
    }
    return client->quickSend<FkGraphicUpdateLayerPrt>(prt, molecule);
}
