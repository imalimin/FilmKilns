/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkRenderEngine.h"
#include "FkRenderDefine.h"
#include "FkRenderMolecule.h"
#include "FkColorCompo.h"
#include "FkSizeCompo.h"
#include "FkFormatCompo.h"
#include "FkNumber.h"
#include "FkWindowProto.h"
#include "FkNewBmpTexProto.h"
#include "FkBufCompo.h"
#include "FkRmMaterialProto.h"

const FkID FkRenderEngine::FK_MSG_RENDER = FK_KID('F', 'R', 'E', 0x01);
const FkID FkRenderEngine::FK_MSG_ADD_MATERIAL = FK_KID('F', 'R', 'E', 0x02);
const FkID FkRenderEngine::FK_MSG_UPDATE_MATERIAL = FK_KID('F', 'R', 'E', 0x03);
const FkID FkRenderEngine::FK_MSG_UPDATE_WINDOW = FK_KID('F', 'R', 'E', 0x04);
const FkID FkRenderEngine::FK_MSG_UPDATE_MATERIAL_WITH_BITMAP = FK_KID('F', 'R', 'E', 0x05);
const FkID FkRenderEngine::FK_MSG_REMOVE_MATERIAL = FK_KID('F', 'R', 'E', 0x06);

FkRenderEngine::FkRenderEngine(std::string name) : FkEngine(name) {
    FK_MARK_SUPER
    FK_REG_MSG(FK_MSG_RENDER, FkRenderEngine::_onRender);
    FK_REG_MSG(FK_MSG_ADD_MATERIAL, FkRenderEngine::_onAddMaterial);
    FK_REG_MSG(FK_MSG_UPDATE_MATERIAL, FkRenderEngine::_onUpdateMaterial);
    FK_REG_MSG(FK_MSG_UPDATE_MATERIAL_WITH_BITMAP, FkRenderEngine::_onUpdateMaterialWithBitmap);
    FK_REG_MSG(FK_MSG_UPDATE_WINDOW, FkRenderEngine::_onUpdateWindow);
    FK_REG_MSG(FK_MSG_REMOVE_MATERIAL, FkRenderEngine::_onRemoveMaterial);
    client = std::make_shared<FkLocalClient>();
    molecule = std::make_shared<FkRenderMolecule>();
}

FkRenderEngine::~FkRenderEngine() {
}

FkResult FkRenderEngine::onCreate() {
    auto ret = FkEngine::onCreate();
    FkAssert(ret == FK_OK, ret);
    auto proto = std::make_shared<FkOnCreatePrt>();
    proto->context = std::make_shared<FkQuarkContext>();
    return client->with(molecule)->send(proto);
}

FkResult FkRenderEngine::onDestroy() {
    auto ret = FkEngine::onDestroy();
    FkAssert(ret == FK_OK, ret);
    return client->quickSend<FkOnDestroyPrt>(molecule);
}

FkResult FkRenderEngine::onStart() {
    auto ret = FkEngine::onStart();
    FkAssert(ret == FK_OK, ret);
    return client->quickSend<FkOnStartPrt>(molecule);
}

FkResult FkRenderEngine::onStop() {
    auto ret = FkEngine::onStop();
    FkAssert(ret == FK_OK, ret);
    return client->quickSend<FkOnStopPrt>(molecule);
}

FkResult FkRenderEngine::renderDevice(std::shared_ptr<FkMaterialEntity> &materials,
                                std::shared_ptr<FkDeviceEntity> &device) {
    if (materials == nullptr || device == nullptr) {
        return FK_NPE;
    }
    auto msg = FkMessage::obtain(FK_MSG_RENDER);
    auto proto = std::make_shared<FkRenderProto>();
    proto->materials = std::make_shared<FkTexEntity>(*materials);
    proto->device = std::move(device);
    msg->sp = std::move(proto);
    return sendMessage(msg);
}

FkResult FkRenderEngine::_onRender(std::shared_ptr<FkMessage> msg) {
    auto sp = std::move(msg->sp);
    auto proto = dynamic_pointer_cast<FkRenderProto>(sp);
    if (proto->env == nullptr) {
        proto->env = std::make_shared<FkEnvEntity>();
    }
    auto ret = client->with(molecule)->send(proto);
    auto bufDevice = std::dynamic_pointer_cast<FkBufDeviceEntity>(proto->device);
    if (bufDevice) {
        bufDevice->finish();
    }
    return ret;
}

std::shared_ptr<FkMaterialCompo> FkRenderEngine::addMaterial() {
    auto msg = FkMessage::obtain(FK_MSG_ADD_MATERIAL);
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

FkResult FkRenderEngine::_onAddMaterial(std::shared_ptr<FkMessage> msg) {
    auto proto = std::make_shared<FkGenIDProto>();
    auto ret = client->with(molecule)->send(proto);
    if (msg->promise != nullptr) {
        msg->promise->set_value(std::make_shared<FkInt>(proto->id));
    }
    return ret;
}

FkResult FkRenderEngine::removeMaterial(std::shared_ptr<FkMaterialCompo> &material) {
    if (material == nullptr) {
        return FK_NPE;
    }
    auto msg = FkMessage::obtain(FK_MSG_REMOVE_MATERIAL);
    msg->sp = material;
    auto ret = sendMessage(msg);
    return ret;
}

FkResult FkRenderEngine::_onRemoveMaterial(std::shared_ptr<FkMessage> msg) {
    auto material = std::dynamic_pointer_cast<FkMaterialCompo>(msg->sp);
    auto proto = std::make_shared<FkRmMaterialProto>(material);
    auto ret = client->with(molecule)->send(proto);
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

FkResult FkRenderEngine::_onUpdateMaterial(std::shared_ptr<FkMessage> &msg) {
    auto proto = std::make_shared<FkNewTexProto>();
    auto material = std::dynamic_pointer_cast<FkMaterialCompo>(msg->sp);
    proto->texEntity = std::make_shared<FkTexEntity>(material);
    proto->texEntity->addComponent(std::make_shared<FkColorCompo>(FkColor::from(msg->arg1)));
    proto->texEntity->addComponent(std::make_shared<FkSizeCompo>(FkSize(msg->arg2)));
    proto->texEntity->addComponent(std::make_shared<FkFormatCompo>(FkColor::kFormat::RGBA));
    return client->with(molecule)->send(proto);
}

FkResult FkRenderEngine::updateMaterialWithBitmap(std::shared_ptr<FkMaterialCompo> &material,
                                                  FkSize size, std::shared_ptr<FkBuffer> buf) {
    auto texEntity = std::make_shared<FkTexEntity>(material);
    texEntity->addComponent(std::make_shared<FkSizeCompo>(size));
    texEntity->addComponent(std::make_shared<FkBufCompo>(buf));
    texEntity->addComponent(std::make_shared<FkFormatCompo>(FkColor::kFormat::RGBA));
    auto msg = FkMessage::obtain(FK_MSG_UPDATE_MATERIAL_WITH_BITMAP);
    msg->sp = texEntity;
    return sendMessage(msg);
}

FkResult FkRenderEngine::_onUpdateMaterialWithBitmap(std::shared_ptr<FkMessage> &msg) {
    auto proto = std::make_shared<FkNewBmpTexProto>();
    proto->texEntity = std::dynamic_pointer_cast<FkTexEntity>(msg->sp);
    return client->with(molecule)->send(proto);
}

FkResult FkRenderEngine::updateWindow(std::shared_ptr<FkGraphicWindow> win) {
    auto msg = FkMessage::obtain(FK_MSG_UPDATE_WINDOW);
    msg->sp = std::move(win);
    return sendMessage(msg);
}

FkResult FkRenderEngine::_onUpdateWindow(shared_ptr<FkMessage> &msg) {
    auto proto = std::make_shared<FkWindowProto>();
    proto->win = std::dynamic_pointer_cast<FkGraphicWindow>(msg->sp);
    return client->with(molecule)->send(proto);
}
