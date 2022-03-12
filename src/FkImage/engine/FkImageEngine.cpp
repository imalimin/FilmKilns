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
#include "FkFilePathCompo.h"
#include "FkGraphicUpdateLayerPrt.h"
#include "FkBitmapCompo.h"
#include "FkFileUtils.h"
#include "FkString.h"

const FkID FkImageEngine::FK_MSG_UPDATE_LAYER_WITH_FILE = 0x100;

FkImageEngine::FkImageEngine(std::shared_ptr<FkEngine> &renderEngine,
                             std::string &workspace,
                             std::string name)
        : FkLayerEngine(renderEngine, name), workspace(workspace) {
    FK_MARK_SUPER
    FK_REG_MSG(FK_MSG_UPDATE_LAYER_WITH_FILE, FkImageEngine::_updateLayerWithFile);
}

FkImageEngine::~FkImageEngine() {
    workspace = "";
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
    if (!FkFileUtils::exist(path)) {
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
    auto layer = std::dynamic_pointer_cast<FkGraphicLayer>(msg->sp);
    auto src = msg->arg3;
    std::string dst = FkString(workspace)
            .append("/layer_")
            .append(layer->id)
            .append(FkFileUtils::suffix(src))
            .toString();
    auto ret = FkFileUtils::copy(src, dst);
    if (FK_OK != ret) {
        FkLogI(FK_DEF_TAG, "Copy source file failed with ret=%d", ret);
        return FK_IO_FAIL;
    }
    auto bmp = FkBitmap::from(dst);
    FkAssert(nullptr != bmp, FK_EMPTY_DATA);
    FkSize size(bmp->getWidth(), bmp->getHeight());
    setCanvasSizeInternal(size, true);

    layer->addComponent(std::make_shared<FkBitmapCompo>(bmp));

    auto proto = std::make_shared<FkGraphicUpdateLayerPrt>();
    proto->layer = layer;
    proto->layer->addComponent(std::make_shared<FkSizeCompo>(size));
    proto->layer->addComponent(std::make_shared<FkFilePathCompo>(dst));
    return getClient()->with(getMolecule())->send(proto);
}
