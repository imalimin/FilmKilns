/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2022-05-26 13:23:41
*/

#include "FkPicModelBuilder.h"
#include "FkFilePathCompo.h"
#include "FkScaleComponent.h"
#include "FkRotateComponent.h"
#include "FkTransComponent.h"
#include "FkSizeCompo.h"
#include "FkColorCompo.h"
#include "FkPathCompo.h"

FK_IMPL_CLASS_TYPE(FkPicModelBuilder, FkObject)

FkPicModelBuilder::FkPicModelBuilder() : FkObject() {
    model = std::make_shared<pb::FkPictureModel>();
}

FkPicModelBuilder::~FkPicModelBuilder() {

}

std::shared_ptr<FkPicModelBuilder> FkPicModelBuilder::newBuilder() {
    return std::make_shared<FkPicModelBuilder>();
}

std::shared_ptr<FkPicModelBuilder> FkPicModelBuilder::setCanvas(std::shared_ptr<FkGraphicLayer> &canvas) {
    auto pb = new pb::FkImageLayer();
    _newImageLayer(pb, canvas);
    model->set_allocated_canvas(pb);
    FkAssert(pb->size().width() != 0, nullptr);
    return shared_from_this();
}

std::shared_ptr<FkPicModelBuilder> FkPicModelBuilder::setLayer(std::shared_ptr<FkGraphicLayer> &layer) {
    auto pb = model->add_layers();
    _newImageLayer(pb, layer);
    FkAssert(pb->size().width() != 0, nullptr);
    return shared_from_this();
}

void FkPicModelBuilder::_newImageLayer(pb::FkImageLayer *pbLayer, std::shared_ptr<FkGraphicLayer> &layer) {
    auto fileCompo = FK_FIND_COMPO(layer, FkFilePathCompo);
    auto sizeCompo = FK_FIND_COMPO(layer, FkSizeCompo);
    auto colorCompo = FK_FIND_COMPO(layer, FkColorCompo);
    pbLayer->set_id(layer->id);
    pbLayer->set_file(fileCompo ? fileCompo->str : "");
    _setMvpInfo(pbLayer, layer);
    _setPathsInfo(pbLayer, layer);
    if (sizeCompo) {
        auto value = new pb::FkSize();
        value->set_width(sizeCompo->size.getWidth());
        value->set_height(sizeCompo->size.getHeight());
        pbLayer->set_allocated_size(value);
    }
    if (colorCompo) {
        pbLayer->set_color(colorCompo->color.toInt());
    }
}

void FkPicModelBuilder::_setMvpInfo(pb::FkImageLayer *pbLayer, std::shared_ptr<FkGraphicLayer> &layer) {
    auto scaleCompo = FK_FIND_COMPO(layer, FkScaleComponent);
    auto rotateCompo = FK_FIND_COMPO(layer, FkRotateComponent);
    auto transCompo = FK_FIND_COMPO(layer, FkTransComponent);
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
}
void FkPicModelBuilder::_setPathsInfo(pb::FkImageLayer *pbLayer, std::shared_ptr<FkGraphicLayer> &layer) {
    std::vector<std::shared_ptr<FkComponent>> paths;
    if (layer->findComponents(paths, FkPathCompo_Class::type) == FK_OK) {
        for (auto &path : paths) {
            auto compo = std::dynamic_pointer_cast<FkPathCompo>(path);
            std::vector<FkDoubleVec2> points;
            if (compo->getPoints(points) > 0) {
                auto pbPath = pbLayer->add_paths();
                pbPath->set_type(compo->getType());
                pbPath->set_color(compo->getColor().toInt());
                for (auto &p : points) {
                    auto vec = pbPath->add_points();
                    vec->set_x(p.x);
                    vec->set_y(p.y);
                    vec->set_z(0.0);
                }
            }
        }
    }
}

std::shared_ptr<pb::FkPictureModel> FkPicModelBuilder::build() {
    return model;
}
