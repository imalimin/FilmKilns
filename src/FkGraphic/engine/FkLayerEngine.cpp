/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkLayerEngine.h"

FkLayerEngine::FkLayerEngine(std::string name) : FkEngine(std::move(name)) {
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

FkID FkLayerEngine::newLayer() {
    return FK_ID_NONE;
}

FkResult FkLayerEngine::_create(std::shared_ptr<AlMessage> msg) {
    return FK_OK;
}

FkResult FkLayerEngine::_destroy(std::shared_ptr<AlMessage> msg) {
    return FK_OK;
}

FkResult FkLayerEngine::_start(std::shared_ptr<AlMessage> msg) {
    return FK_OK;
}

FkResult FkLayerEngine::_stop(std::shared_ptr<AlMessage> msg) {
    return FK_OK;
}

FkResult FkLayerEngine::_newLayer(std::shared_ptr<AlMessage> msg) {
    return FK_OK;
}