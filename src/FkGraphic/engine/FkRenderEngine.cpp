/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkRenderEngine.h"
#include "FkRenderDefine.h"
#include "FkRenderMolecule.h"
#include "FkNumber.h"

const FkID FkRenderEngine::FK_MSG_RENDER = FK_KID('F', 'R', 'E', 0x01);
const FkID FkRenderEngine::FK_MSG_NEW_MATERIAL = FK_KID('F', 'R', 'E', 0x02);
const FkID FkRenderEngine::FK_MSG_UPDATE_MATERIAL = FK_KID('F', 'R', 'E', 0x03);

FkRenderEngine::FkRenderEngine(std::string name) : FkEngine(name) {
    FK_MARK_SUPER
    FK_REG_MSG(FK_MSG_RENDER, FkRenderEngine::_onRender);
    FK_REG_MSG(FK_MSG_NEW_MATERIAL, FkRenderEngine::_onNewMaterial);
    FK_REG_MSG(FK_MSG_UPDATE_MATERIAL, FkRenderEngine::_onUpdateMaterial);
    client = std::make_shared<FkLocalClient>();
    molecule = std::make_shared<FkRenderMolecule>();
}

FkRenderEngine::~FkRenderEngine() {

}

FkResult FkRenderEngine::onCreate() {
    auto ret = FkEngine::onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    client->quickSend<FkOnCreatePrt>(molecule);
    return ret;
}

FkResult FkRenderEngine::onDestroy() {
    auto ret = FkEngine::onDestroy();
    if (FK_OK != ret) {
        return ret;
    }
    client->quickSend<FkOnDestroyPrt>(molecule);
    return ret;
}

FkResult FkRenderEngine::onStart() {
    auto ret = FkEngine::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    client->quickSend<FkOnStartPrt>(molecule);
    return ret;
}

FkResult FkRenderEngine::onStop() {
    auto ret = FkEngine::onStop();
    if (FK_OK != ret) {
        return ret;
    }
    client->quickSend<FkOnStopPrt>(molecule);
    return ret;
}

FkResult FkRenderEngine::render() {
    auto msg = FkMessage::obtain(FK_MSG_RENDER);
    return sendMessage(msg);
}

FkResult FkRenderEngine::_onRender(std::shared_ptr<FkMessage> msg) {
    auto proto = std::make_shared<FkRenderProto>();
    return client->with(molecule)->send(proto);
}

std::shared_ptr<FkMaterialCompo> FkRenderEngine::newMaterial() {
    auto msg = FkMessage::obtain(FK_MSG_NEW_MATERIAL);
    msg->promise = std::make_shared<std::promise<std::shared_ptr<FkObject>>>();
    auto ret = sendMessage(msg);
    std::shared_ptr<FkObject> result = nullptr;
    if (FK_OK == ret && (result = msg->promise->get_future().get())) {
        auto value = std::static_pointer_cast<FkInt>(result);
        if (value) {
            return std::make_shared<FkMaterialCompo>(value->get());
        }
    }
    return std::make_shared<FkMaterialCompo>(FK_ID_NONE);
}

FkResult FkRenderEngine::_onNewMaterial(std::shared_ptr<FkMessage> msg) {
    auto proto = std::make_shared<FkGenIDProto>();
    auto ret = client->with(molecule)->send(proto);
    if (msg->promise != nullptr) {
        msg->promise->set_value(std::make_shared<FkInt>(proto->id));
    }
    return ret;
}

FkResult FkRenderEngine::updateMaterial(shared_ptr<FkMaterialCompo> &material,
                                        FkSize size, FkColor color) {
    auto msg = FkMessage::obtain(FK_MSG_UPDATE_MATERIAL);
    msg->sp = material;
    msg->arg1 = color.toInt();
    msg->arg2 = size.toInt64();
    return sendMessage(msg);
}

FkResult FkRenderEngine::_onUpdateMaterial(std::shared_ptr<FkMessage> msg) {
    shared_ptr<FkMaterialCompo> material = std::dynamic_pointer_cast<FkMaterialCompo>(msg->sp);
    FkColor color = FkColor::from(msg->arg1);
    FkSize size = FkSize(msg->arg2);
    auto proto = std::make_shared<FkNewTexProto>();
    proto->texEntity = std::make_shared<FkTexEntity>();
    return client->with(molecule)->send(proto);
}
