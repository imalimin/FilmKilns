/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkImageEngine.h"
#include "FkGraphicNewTexPtl.h"
#include "FkGraphicLayer.h"
#include "FkBitmap.h"
#include "FkUpdateTexWithBmpPrt.h"
#include "FkDefinition.h"
#include "FkSizeComponent.h"
#include "FkTexIDComponent.h"
#include "FkGraphicUpdateLayerPrt.h"
#include <zconf.h>

const FkID FkImageEngine::FK_MSG_UPDATE_LAYER_WITH_FILE = FK_KID('F', 'K', 'I', 0x10);

FkImageEngine::FkImageEngine(std::string name) : FkLayerEngine(std::move(name)) {
    FK_MARK_SUPER
    FK_REG_MSG(FK_MSG_UPDATE_LAYER_WITH_FILE, FkImageEngine::_updateLayerWithFile);
}

FkImageEngine::~FkImageEngine() {

}

FkResult FkImageEngine::onCreate() {
    return FkLayerEngine::onCreate();
}

FkResult FkImageEngine::onDestroy() {
    return FkLayerEngine::onDestroy();
}

FkResult FkImageEngine::onStart() {
    return FkLayerEngine::onStart();
}

FkResult FkImageEngine::onStop() {
    return FkLayerEngine::onStop();
}

FkID FkImageEngine::newLayerWithFile(std::string path) {
    if (access(path.c_str(), F_OK) == -1) {
        FkLogE("File not found: %s", path.c_str());
        return FK_ID_NONE;
    }
    auto id = newLayer();
    if (FK_ID_NONE != id) {
        auto layer = std::make_shared<FkGraphicLayer>();
        layer->id = id;
        auto msg = FkMessage::obtain(FK_MSG_UPDATE_LAYER_WITH_FILE);
        msg->arg3 = std::move(path);
        msg->sp = layer;
        sendMessage(msg);
    }
    return id;
}

FkResult FkImageEngine::_updateLayerWithFile(std::shared_ptr<FkMessage> msg) {
    auto bmp = FkBitmap::from(msg->arg3);
    FkAssert(nullptr != bmp, FK_EMPTY_DATA);
    FkSize canvasSize(bmp->getWidth(), bmp->getHeight());
    setCanvasSizeInternal(canvasSize);

    auto texPrt = std::make_shared<FkGraphicNewTexPtl>();
    texPrt->fmt = FkColor::kFormat::RGBA;
    FkAssert(FK_OK == getClient()->quickSend(texPrt, getMolecule()), FK_EMPTY_DATA);

    auto layer = Fk_POINTER_CAST(FkGraphicLayer, msg->sp);
    auto updatePrt = std::make_shared<FkUpdateTexWithBmpPrt>();
    updatePrt->id = texPrt->id;
    updatePrt->bmp = bmp;
    FkAssert(FK_OK == getClient()->quickSend(updatePrt, getMolecule()), FK_FAIL);

    auto com = std::make_shared<FkTexIDComponent>();
    com->id = texPrt->id;
    auto sizeCom = std::make_shared<FkSizeComponent>();
    sizeCom->size.set(updatePrt->bmp->getWidth(), updatePrt->bmp->getHeight());

    auto prt = std::make_shared<FkGraphicUpdateLayerPrt>();
    prt->layer = layer;
    prt->layer->addComponent(com);
    prt->layer->addComponent(sizeCom);
    return getClient()->quickSend<FkGraphicUpdateLayerPrt>(prt, getMolecule());
}
