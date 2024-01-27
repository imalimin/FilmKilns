/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: ${author}
* CREATE TIME: 2022-3-8 12:58:51
*/

#include "FkImageModelEngine.h"
#include "FkImageEngine.h"
#include "FkFilePathCompo.h"
#include "FkScaleComponent.h"
#include "FkRotateComponent.h"
#include "FkTransComponent.h"
#include "FkSizeCompo.h"
#include "FkFileUtils.h"
#include "FkAnyCompo.h"
#include "FkColorCompo.h"
#include "FkPathCompo.h"
#include "FkPicModelBuilder.h"
#include "FkPbModel.h"
#include "FkFuncCompo.h"
#include <fstream>

FK_IMPL_CLASS_TYPE(FkImageModelEngine, FkEngine)

FkImageModelEngine::FkImageModelEngine(std::shared_ptr<FkImageEngine> &imageEngine, std::string name)
        : FkEngine(name), imageEngine(imageEngine) {

}

FkImageModelEngine::~FkImageModelEngine() {

}

FkResult FkImageModelEngine::onCreate() {
    auto ret = FkEngine::onCreate();
    return ret;
}

FkResult FkImageModelEngine::onDestroy() {
    auto ret = FkEngine::onDestroy();
    return ret;
}

FkResult FkImageModelEngine::onStart() {
    auto ret = FkEngine::onStart();
    return ret;
}

FkResult FkImageModelEngine::onStop() {
    auto ret = FkEngine::onStop();
    return ret;
}

FkResult FkImageModelEngine::save(std::string &file) {
    std::vector<std::shared_ptr<FkGraphicLayer>> layers;
    auto ret = imageEngine->queryLayers(layers);
    if (FK_OK != ret) {
        FkLogI(FK_DEF_TAG, "Query layers fail with %d", ret);
        return FK_INVALID_DATA;
    }
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkImageModelEngine::_save));
    msg->arg3 = file;
    msg->sp = std::make_shared<FkAnyCompo>(layers);
    return sendMessage(msg);
}

FkResult FkImageModelEngine::_save(std::shared_ptr<FkMessage> &msg) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(anyCompo, FkAnyCompo, msg->sp);
    FkAssert(anyCompo->any.has_value(), FK_FAIL);
    auto layers = std::any_cast<std::vector<std::shared_ptr<FkGraphicLayer>>>(anyCompo->any);
    auto dir = msg->arg3;
    FkAssert(FkFileUtils::exist(dir), FK_FILE_NOT_FOUND);
    auto model = convert2PictureModel(dir, layers);
    return _writeModel2File(dir, model);
}

FkResult FkImageModelEngine::load(std::string &file, std::function<void()> finishCallback) {
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkImageModelEngine::_load));
    msg->arg3 = file;
    msg->sp = std::make_shared<FkFuncCompo>(finishCallback);
    return sendMessage(msg);
}

FkResult FkImageModelEngine::_load(std::shared_ptr<FkMessage> &msg) {
    auto engine = imageEngine;
    auto dir = msg->arg3;
    FkAssert(FkFileUtils::exist(dir), FK_FILE_NOT_FOUND);
    auto file = dir;
    file.append("/model.pb");
    std::fstream stream;
    stream.open(file.c_str(), std::ios::in | std::ios::binary);
    auto model = std::make_shared<pb::FkPictureModel>();
    if (!model->ParseFromIstream(&stream)) {
        FkLogE(FK_DEF_TAG, "Parse proto buffer fail.");
        return FK_IO_FAIL;
    }

    /// Remove all layers and canvas.
    engine->removeLayer(Fk_CANVAS_ID);
    for (auto &layer : model->layers()) {
        FkID layerID = FK_ID_NONE;
        if (!layer.file().empty()) {
            auto layerFile = dir;
            FkFileUtils::trim(layerFile);
            layerFile.append("/").append(layer.file());
            layerID = engine->newLayerWithFile(layerFile, layer.id());
        } else {
            layerID = engine->newLayerWithColor(FkSize(layer.size().width(), layer.size().height()),
                                      FkColor::makeFrom(layer.color()),
                                      layer.id());
        }
        FkAssert(layerID > 0, FK_FAIL);
        std::shared_ptr<FkModelInterface> modelInterface = std::make_shared<FkPbModel>(model, layer);
        engine->updateLayerWithModel(layerID, modelInterface);
    }
    auto canvasSize = model->canvas().size();
    FkAssert(canvasSize.width() != 0 && canvasSize.height() != 0, FK_FAIL);
    engine->setCanvasSize(FkSize(canvasSize.width(), canvasSize.height()));
    engine->notifyRender();
    if (msg->sp) {
        FK_CAST_NULLABLE_PTR_RETURN_INT(finishCompo, FkFuncCompo, msg->sp);
        finishCompo->func();
    }
    return FK_OK;
}

FkResult FkImageModelEngine::getLayers(FkImageModelEngine::FkModelCallback callback) {
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkImageModelEngine::_getLayers));
    msg->sp = std::make_shared<FkAnyCompo>(callback);
    return sendMessage(msg);
}

FkResult FkImageModelEngine::_getLayers(std::shared_ptr<FkMessage> &msg) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(compo, FkAnyCompo, msg->sp);
    std::vector<std::shared_ptr<FkGraphicLayer>> layers;
    auto ret = imageEngine->queryLayers(layers);

    std::string dir("");
    auto model = convert2PictureModel(dir, layers);
    if (compo->any.has_value()) {
        std::any_cast<FkImageModelEngine::FkModelCallback>(compo->any)(model);
        return FK_OK;
    }
    return FK_FAIL;
}

FkResult FkImageModelEngine::getLayer(FkID layer, FkImageModelEngine::FkModelCallback callback) {
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkImageModelEngine::_getLayer));
    msg->arg1 = layer;
    msg->any = callback;
    return sendMessage(msg);
}

FkResult FkImageModelEngine::_getLayer(std::shared_ptr<FkMessage> &msg) {
    return 0;
}

std::shared_ptr<pb::FkPictureModel> FkImageModelEngine::convert2PictureModel(std::string &dir,
                                                                             std::vector<std::shared_ptr<FkGraphicLayer>> &layers) {
    auto builder = FkPicModelBuilder::newBuilder();
    for (auto &layer : layers) {
        if (layer->id == Fk_CANVAS_ID) {
            builder->setCanvas(layer);
        } else {
            builder->setLayer(layer);
        }
    }
    return builder->build();
}

FkResult FkImageModelEngine::_writeModel2File(std::string &dir,
                                              std::shared_ptr<pb::FkPictureModel> &model) {
    auto modelFile = dir;
    modelFile.append("/model.pb");
    if (FkFileUtils::exist(modelFile)) {
        FkFileUtils::remove(modelFile);
    }
    std::fstream stream;
    stream.open(modelFile.c_str(), std::ios::out | std::ios::binary);
    auto ret = model->SerializeToOstream(&stream);
    stream.flush();
    stream.close();
    if (!ret) {
        FkLogI(FK_DEF_TAG, "Write pb model fail with code.");
    }
    FkAssert(ret, FK_IO_FAIL);
    return ret ? FK_OK : FK_IO_FAIL;
}
