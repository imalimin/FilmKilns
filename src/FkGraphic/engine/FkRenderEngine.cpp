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

FkRenderEngine::FkRenderEngine(std::string name) : FkEngine(name) {
    FK_MARK_SUPER
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
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkRenderEngine::_onRender));
    auto proto = std::make_shared<FkRenderProto>();
    proto->materials = std::make_shared<FkTexEntity>(*materials);
    proto->device = std::move(device);
    msg->sp = std::move(proto);
    return sendMessage(msg);
}

FkResult FkRenderEngine::_onRender(std::shared_ptr<FkMessage> msg) {
    auto sp = std::move(msg->sp);
    auto proto = std::dynamic_pointer_cast<FkRenderProto>(sp);
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
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkRenderEngine::_onAddMaterial));
    msg->withPromise();
    auto ret = sendMessage(msg);
    FkID id = FK_ID_NONE;
    if (FK_OK == ret) {
        msg->getPromiseResult(id);
    }
    return std::make_shared<FkMaterialCompo>(id);
}

FkResult FkRenderEngine::_onAddMaterial(std::shared_ptr<FkMessage> msg) {
    auto proto = std::make_shared<FkGenIDProto>();
    auto ret = client->with(molecule)->send(proto);
    msg->setPromiseResult(proto->id);
    return ret;
}

FkResult FkRenderEngine::removeMaterial(std::shared_ptr<FkMaterialCompo> &material) {
    if (material == nullptr) {
        return FK_NPE;
    }
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkRenderEngine::_onRemoveMaterial));
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

FkResult FkRenderEngine::updateMaterial(std::shared_ptr<FkMaterialCompo> &material,
                                        FkSize size, FkColor color) {
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkRenderEngine::_onUpdateMaterial));
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
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkRenderEngine::_onUpdateMaterialWithBitmap));
    msg->sp = texEntity;
    return sendMessage(msg);
}

FkResult FkRenderEngine::_onUpdateMaterialWithBitmap(std::shared_ptr<FkMessage> &msg) {
    auto proto = std::make_shared<FkNewBmpTexProto>();
    proto->texEntity = std::dynamic_pointer_cast<FkTexEntity>(msg->sp);
    return client->with(molecule)->send(proto);
}

FkResult FkRenderEngine::updateWindow(std::shared_ptr<FkGraphicWindow> win) {
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkRenderEngine::_onUpdateWindow));
    msg->sp = std::move(win);
    return sendMessage(msg);
}

FkResult FkRenderEngine::_onUpdateWindow(std::shared_ptr<FkMessage> &msg) {
    auto proto = std::make_shared<FkWindowProto>();
    proto->win = std::dynamic_pointer_cast<FkGraphicWindow>(msg->sp);
    return client->with(molecule)->send(proto);
}
