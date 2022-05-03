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
#include <fstream>

FK_IMPL_CLASS_TYPE(FkImageModelEngine, FkEngine)

static std::shared_ptr<FkImageEngine> _cast2ImageEngine(std::shared_ptr<FkEngine> &imageEngine) {
    return std::dynamic_pointer_cast<FkImageEngine>(imageEngine);
}

FkImageModelEngine::FkImageModelEngine(std::shared_ptr<FkEngine> &imageEngine, std::string name)
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
    auto ret = _cast2ImageEngine(imageEngine)->queryLayers(layers);
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
    auto file = msg->arg3;
    auto dir = _createTempDir(file);
    auto model = convert2PictureModel(dir, layers);
    return _writeModel2File(dir, model);
}

FkResult FkImageModelEngine::load(std::string &file) {
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkImageModelEngine::_load));
    msg->arg3 = file;
    return sendMessage(msg);
}

FkResult FkImageModelEngine::_load(std::shared_ptr<FkMessage> &msg) {
    auto engine = _cast2ImageEngine(imageEngine);
    auto dir = msg->arg3;
    dir.append(".dir");
    auto file = dir;
    file.append("/model.pb");
    std::fstream stream;
    stream.open(file.c_str(), std::ios::in | std::ios::binary);
    auto model = std::make_shared<pb::FkPictureModel>();
    if (!model->ParseFromIstream(&stream)) {
        FkLogE(FK_DEF_TAG, "Parse proto buffer fail.");
        return FK_IO_FAIL;
    }
    for (auto &layer : model->layers()) {
        if (!layer.file().empty()) {
            auto layerFile = dir;
            layerFile.append(layer.file());
            engine->newLayerWithFile(layerFile, layer.id());
        } else {
            engine->newLayerWithColor(FkSize(layer.size().width(), layer.size().height()),
                                      FkColor::makeFrom(layer.color()),
                                      layer.id());
        }
        engine->setTranslate(layer.id(), layer.trans().x(), layer.trans().y());
        engine->setScale(layer.id(), layer.scale().x(), layer.scale().y());
        FkRational rational(layer.rotation().num(), layer.rotation().den());
        engine->setRotation(layer.id(), rational);
    }
    auto canvasSize = model->canvas().size();
    FkAssert(canvasSize.width() != 0 && canvasSize.height() != 0, FK_FAIL);
    engine->setCanvasSize(FkSize(canvasSize.width(), canvasSize.height()));
    engine->notifyRender();
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
    auto ret = _cast2ImageEngine(imageEngine)->queryLayers(layers);

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
    auto model = std::make_shared<pb::FkPictureModel>();
    for (auto &layer : layers) {
        if (layer->id == Fk_CANVAS_ID) {
            auto canvas = new pb::FkImageLayer();
            model->set_allocated_canvas(canvas);
            _fillLayer(canvas, layer);
            FkAssert(canvas->size().width() != 0, nullptr);
        } else {
            auto fileCompo = FK_FIND_COMPO(layer, FkFilePathCompo);
            if (fileCompo && !dir.empty()) {
                _copyLayerFile(dir, fileCompo->str);
            }
            auto pbLayer = model->add_layers();
            _fillLayer(pbLayer, layer);
        }
    }
    return model;
}

FkResult FkImageModelEngine::_fillLayer(void* dst,
                                        std::shared_ptr<FkGraphicLayer> &src) {
    auto *pbLayer = static_cast<pb::FkImageLayer *>(dst);
    auto fileCompo = FK_FIND_COMPO(src, FkFilePathCompo);
    auto scaleCompo = FK_FIND_COMPO(src, FkScaleComponent);
    auto rotateCompo = FK_FIND_COMPO(src, FkRotateComponent);
    auto transCompo = FK_FIND_COMPO(src, FkTransComponent);
    auto sizeCompo = FK_FIND_COMPO(src, FkSizeCompo);
    auto colorCompo = FK_FIND_COMPO(src, FkColorCompo);
    pbLayer->set_id(src->id);
    pbLayer->set_file(fileCompo ? FkFileUtils::name(fileCompo->str) : "");
    if (scaleCompo) {
        auto value = new pb::FkFloatVec3();
        value->set_x(scaleCompo->value.x);
        value->set_y(scaleCompo->value.y);
        value->set_z(scaleCompo->value.z);
        pbLayer->set_allocated_scale(value);
    }
    if (rotateCompo) {
        auto value = new pb::FkRational();
        value->set_num(rotateCompo->value.num);
        value->set_den(rotateCompo->value.den);
        pbLayer->set_allocated_rotation(value);
    }
    if (transCompo) {
        auto value = new pb::FkIntVec3();
        value->set_x(transCompo->value.x);
        value->set_y(transCompo->value.y);
        value->set_z(0);
        pbLayer->set_allocated_trans(value);
    }
    if (sizeCompo) {
        auto value = new pb::FkSize();
        value->set_width(sizeCompo->size.getWidth());
        value->set_height(sizeCompo->size.getHeight());
        pbLayer->set_allocated_size(value);
    }
    if (colorCompo) {
        pbLayer->set_color(colorCompo->color.toInt());
    }
    return FK_OK;
}

std::string FkImageModelEngine::_createTempDir(std::string &file) {
    auto dir = file;
    dir.append(".dir");
    FkFileUtils::mkdirs(dir);
    return dir;
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

FkResult FkImageModelEngine::_copyLayerFile(std::string &dir, std::string &src) {
    std::string dst = dir;
    dst.append("/");
    dst.append(FkFileUtils::name(src));
    return FkFileUtils::copy(src, dst);
}
