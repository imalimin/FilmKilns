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
#include "FkReadPixelsProto.h"

FkImageEngine::FkImageEngine(std::shared_ptr<FkEngine> &renderEngine,
                             std::string &workspace,
                             std::string name)
        : FkLayerEngine(renderEngine, name), workspace(workspace) {
    FK_MARK_SUPER
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

FkID FkImageEngine::newLayerWithFile(std::string path, FkID expectId) {
    if (!FkFileUtils::exist(path)) {
        FkLogE("File not found: %s", path.c_str());
        return FK_ID_NONE;
    }
    auto id = newLayer(expectId);
    if (FK_ID_NONE != id) {
        auto layer = std::make_shared<FkGraphicLayer>();
        layer->id = id;
        auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkImageEngine::_updateLayerWithFile));
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
    FkSize layerSize(bmp->getWidth(), bmp->getHeight());
    FkSize canvasSize(layerSize);
    if (bmp->isSwappedWH()) {
        canvasSize.swap();
    }
    setCanvasSizeInternal(canvasSize, true);

    layer->addComponent(std::make_shared<FkBitmapCompo>(bmp));

    auto proto = std::make_shared<FkGraphicUpdateLayerPrt>();
    proto->layer = layer;
    proto->layer->addComponent(std::make_shared<FkSizeCompo>(layerSize));
    proto->layer->addComponent(std::make_shared<FkFilePathCompo>(dst));
    return getClient()->with(getMolecule())->send(proto);
}

FkID FkImageEngine::save(std::string file) {
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkImageEngine::_save));
    msg->arg3 = file;
    return sendMessage(msg);
}

FkResult FkImageEngine::_save(std::shared_ptr<FkMessage> msg) {
    auto proto = std::make_shared<FkReadPixelsProto>();
    proto->layerId = Fk_CANVAS_ID;
    auto ret = getClient()->with(getMolecule())->send(proto);
    if (FK_OK != ret) {
        return ret;
    }
    if (proto->buf == nullptr) {
        return FK_EMPTY_DATA;
    } else {
        FkImage::Format fmt;
        std::string suffix = FkFileUtils::suffix(msg->arg3);
        if (suffix == ".jpeg" || suffix == ".jpg") {
            fmt = FkImage::Format::kJPEG;
        } else if (suffix == ".png") {
            fmt = FkImage::Format::kPNG;
        } else if (suffix == ".bmp") {
            fmt = FkImage::Format::kBMP;
        } else if (suffix == ".webp") {
            fmt = FkImage::Format::kWEBP;
        } else {
            fmt = FkImage::Format::kJPEG;
        }
        ret = FkBitmap::write(msg->arg3, fmt, proto->buf, proto->size, 80);
    }
    return ret;
}
