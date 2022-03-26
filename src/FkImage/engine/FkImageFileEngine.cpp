/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: ${author}
* CREATE TIME: 2022-3-8 12:58:51
*/

#include "FkImageFileEngine.h"
#include "FkImageEngine.h"
#include "FkFilePathCompo.h"
#include "FkScaleComponent.h"
#include "FkRotateComponent.h"
#include "FkTransComponent.h"
#include "FkSizeCompo.h"
#include "FkPictureModel.pb.h"
#include "FkFileUtils.h"
#include <fstream>

static std::shared_ptr<FkImageEngine> _cast2ImageEngine(std::shared_ptr<FkEngine> &imageEngine) {
    return std::dynamic_pointer_cast<FkImageEngine>(imageEngine);
}

FkImageFileEngine::FkImageFileEngine(std::shared_ptr<FkEngine> &imageEngine, std::string name)
        : FkEngine(name), imageEngine(imageEngine) {
    FK_MARK_SUPER
}

FkImageFileEngine::~FkImageFileEngine() {

}

FkResult FkImageFileEngine::onCreate() {
    auto ret = FkEngine::onCreate();
    return ret;
}

FkResult FkImageFileEngine::onDestroy() {
    auto ret = FkEngine::onDestroy();
    return ret;
}

FkResult FkImageFileEngine::onStart() {
    auto ret = FkEngine::onStart();
    return ret;
}

FkResult FkImageFileEngine::onStop() {
    auto ret = FkEngine::onStop();
    return ret;
}

FkResult FkImageFileEngine::save(std::string &file) {
    layers.clear();
    auto ret = _cast2ImageEngine(imageEngine)->queryLayers(layers);

    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkImageFileEngine::_save));
    msg->arg3 = file;
    return sendMessage(msg);
}

FkResult FkImageFileEngine::_save(std::shared_ptr<FkMessage> &msg) {
    auto file = msg->arg3;
    auto dir = _createTempDir(file);
    auto model = std::make_shared<fk_pb::FkPictureModel>();
    for (auto &layer : layers) {
        if (layer->id == Fk_CANVAS_ID) {
            auto canvas = new fk_pb::FkImageLayer();
            model->set_allocated_canvas(canvas);
            _fillLayer(canvas, layer);
        } else {
            auto fileCompo = layer->findComponent<FkFilePathCompo>();
            if (fileCompo) {
                _copyLayerFile(dir, fileCompo->str);
            }
            auto pbLayer = model->add_layers();
            _fillLayer(pbLayer, layer);
        }
    }

    return _writeModel2File(dir, model);
}

FkResult FkImageFileEngine::load(std::string &file) {
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkImageFileEngine::_load));
    msg->arg3 = file;
    return sendMessage(msg);
}

FkResult FkImageFileEngine::_load(std::shared_ptr<FkMessage> &msg) {
    auto engine = _cast2ImageEngine(imageEngine);
    auto dir = msg->arg3;
    dir.append(".dir");
    auto file = dir;
    file.append("/model.pb");
    std::fstream stream;
    stream.open(file.c_str(), std::ios::in | std::ios::binary);
    auto model = std::make_shared<fk_pb::FkPictureModel>();
    model->ParseFromIstream(&stream);
    for (auto &layer : model->layers()) {
        if (!layer.file().empty()) {
            auto layerFile = dir;
            layerFile.append(layer.file());
            engine->newLayerWithFile(layerFile, layer.id());
            engine->setTranslate(layer.id(), layer.trans().x(), layer.trans().y());
            engine->setScale(layer.id(), layer.scale().x(), layer.scale().y());
            FkRational rational(layer.rotation().num(), layer.rotation().den());
            engine->setRotation(layer.id(), rational);
        }
    }
    auto canvasSize = model->canvas().size();
    engine->setCanvasSize(FkSize(canvasSize.width(), canvasSize.height()));
    return FK_OK;
}

FkResult FkImageFileEngine::_fillLayer(void* dst,
                                      std::shared_ptr<FkGraphicLayer> &src) {
    auto *pbLayer = static_cast<fk_pb::FkImageLayer *>(dst);
    auto fileCompo = src->findComponent<FkFilePathCompo>();
    auto scaleCompo = src->findComponent<FkScaleComponent>();
    auto rotateCompo = src->findComponent<FkRotateComponent>();
    auto transCompo = src->findComponent<FkTransComponent>();
    auto sizeCompo = src->findComponent<FkSizeCompo>();
    pbLayer->set_id(src->id);
    pbLayer->set_file(fileCompo ? FkFileUtils::name(fileCompo->str) : "");
    if (scaleCompo) {
        auto value = new fk_pb::FkFloatVec3();
        value->set_x(scaleCompo->value.x);
        value->set_y(scaleCompo->value.y);
        value->set_z(scaleCompo->value.z);
        pbLayer->set_allocated_scale(value);
    }
    if (rotateCompo) {
        auto value = new fk_pb::FkRational();
        value->set_num(rotateCompo->value.num);
        value->set_den(rotateCompo->value.den);
        pbLayer->set_allocated_rotation(value);
    }
    if (transCompo) {
        auto value = new fk_pb::FkIntVec3();
        value->set_x(transCompo->value.x);
        value->set_y(transCompo->value.y);
        value->set_z(0);
        pbLayer->set_allocated_trans(value);
    }
    if (sizeCompo) {
        auto value = new fk_pb::FkSize();
        value->set_width(sizeCompo->size.getWidth());
        value->set_height(sizeCompo->size.getHeight());
        pbLayer->set_allocated_size(value);
    }
    return FK_OK;
}

std::string FkImageFileEngine::_createTempDir(std::string &file) {
    auto dir = file;
    dir.append(".dir");
    FkFileUtils::mkdirs(dir);
    return dir;
}

FkResult FkImageFileEngine::_writeModel2File(std::string &dir, std::any model) {
    auto modelFile = dir;
    modelFile.append("/model.pb");
    std::fstream stream;
    stream.open(modelFile.c_str(), std::ios::out | std::ios::binary);
    return std::any_cast<std::shared_ptr<fk_pb::FkPictureModel>>(model)
                   ->SerializeToOstream(&stream) ? FK_OK : FK_IO_FAIL;
}

FkResult FkImageFileEngine::_copyLayerFile(std::string &dir, std::string &src) {
    std::string dst = dir;
    dst.append("/");
    dst.append(FkFileUtils::name(src));
    return FkFileUtils::copy(src, dst);
}
