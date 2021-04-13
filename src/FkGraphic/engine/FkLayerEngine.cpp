/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkLayerEngine.h"
#include "FkGraphicNewLayerPrt.h"

const FkID FkLayerEngine::FK_MSG_NEW_LAYER = FK_KID('F', 'K', 'E', 0x10);

FkLayerEngine::FkLayerEngine(std::string name) : FkEngine(std::move(name)) {
    FK_REG_MSG(FK_MSG_NEW_LAYER, FkLayerEngine::_newLayer)
    client = std::make_shared<FkLocalClient>();
    molecule = std::make_shared<FkGraphicMolecule>();
}

FkLayerEngine::~FkLayerEngine() {

}

FkResult FkLayerEngine::create() {
    auto ret = FkEngine::create();
    if(FK_OK!= ret) {
        return ret;
    }
    client->quickSend<FkOnCreatePrt>(molecule);
    return ret;
}

FkResult FkLayerEngine::destroy() {
    auto ret = FkEngine::destroy();
    if(FK_OK!= ret) {
        return ret;
    }
    client->quickSend<FkOnDestroyPrt>(molecule);
    return ret;
}

FkResult FkLayerEngine::start() {
    auto ret = FkEngine::start();
    if(FK_OK!= ret) {
        return ret;
    }
    client->quickSend<FkOnStartPrt>(molecule);
    return ret;
}

FkResult FkLayerEngine::stop() {
    auto ret = FkEngine::stop();
    if(FK_OK!= ret) {
        return ret;
    }
    client->quickSend<FkOnStopPrt>(molecule);
    return ret;
}

FkID FkLayerEngine::newLayer(std::string path) {
    auto msg = FkMessage::obtain(FK_MSG_NEW_LAYER);
    msg->arg3 = std::move(path);
    msg->promise = std::make_shared<std::promise<std::shared_ptr<FkObject>>>();
    sendMessage(msg);
    std::shared_ptr<FkGraphicNewLayerPrt> result = std::static_pointer_cast<FkGraphicNewLayerPrt>(msg->promise->get_future().get());
    if (result->layer) {
        return result->layer->id;
    }
    return FK_ID_NONE;
}

FkResult FkLayerEngine::_newLayer(std::shared_ptr<FkMessage> msg) {
    auto prt = std::make_shared<FkGraphicNewLayerPrt>();
    auto ret = client->quickSend<FkGraphicNewLayerPrt>(prt, molecule);
    msg->promise->set_value(prt);
    return ret;
}