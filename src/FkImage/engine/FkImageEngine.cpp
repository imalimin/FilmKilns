/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkImageEngine.h"
#include "FkGraphicLayer.h"
#include "FkBitmap.h"
#include "FkDefinition.h"
#include "FkSizeCompo.h"
#include "FkGraphicUpdateLayerPrt.h"
#include "FkBitmapCompo.h"
#include <zconf.h>

const FkID FkImageEngine::FK_MSG_UPDATE_LAYER_WITH_FILE = FK_KID('F', 'K', 'I', 0x10);

FkImageEngine::FkImageEngine(std::shared_ptr<FkEngine> &renderEngine, std::string name)
        : FkLayerEngine(renderEngine, name) {
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
        msg->arg3 = path;
        msg->sp = layer;
        sendMessage(msg);
    }
    return id;
}

FkResult FkImageEngine::_updateLayerWithFile(std::shared_ptr<FkMessage> msg) {
    auto bmp = FkBitmap::from(msg->arg3);
    FkAssert(nullptr != bmp, FK_EMPTY_DATA);
    FkSize size(bmp->getWidth(), bmp->getHeight());
    setCanvasSizeInternal(size, true);

    auto layer = std::dynamic_pointer_cast<FkGraphicLayer>(msg->sp);
    layer->addComponent(std::make_shared<FkBitmapCompo>(bmp));

    auto proto = std::make_shared<FkGraphicUpdateLayerPrt>();
    proto->layer = layer;
    proto->layer->addComponent(std::make_shared<FkSizeCompo>(size));
    return getClient()->with(getMolecule())->send(proto);
}
