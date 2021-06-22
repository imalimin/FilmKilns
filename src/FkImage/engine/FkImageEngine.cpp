/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkImageEngine.h"
#include "AlBitmapFactory.h"
#include "FkGraphicNewTexPtl.h"
#include "FkGraphicLayer.h"
#include "FkBitmap.h"
#include "FkUpdateTexWithBmpPrt.h"

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
    auto texPrt = std::make_shared<FkGraphicNewTexPtl>();
    texPrt->fmt = FkColor::kFormat::RGBA;
    auto ret = getClient()->quickSend<FkGraphicNewTexPtl>(texPrt, getMolecule());
    if (FK_OK == ret) {
        auto layer = Fk_POINTER_CAST(FkGraphicLayer, msg->sp);
        auto bmp = FkBitmap::from(AlBitmapFactory::decodeFile(msg->arg3));
        auto updatePrt = std::make_shared<FkUpdateTexWithBmpPrt>();
        updatePrt->id=texPrt->id;
        updatePrt->pixels = bmp->getPixels();
    }
    return FK_FAIL;
}
