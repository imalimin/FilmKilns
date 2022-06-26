/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicLayer.h"
#include "FkSizeCompo.h"
#include "FkScaleComponent.h"
#include "FkTransComponent.h"
#include "FkRotateComponent.h"

FK_IMPL_CLASS_TYPE(FkGraphicLayer, FkGraphicEntity)

float FkGraphicLayer::calcScaleWithScaleType(FkSize &src,
                                             FkSize &dst,
                                             kScaleType scaleType) {
    float scale = 1.0f;
    switch (scaleType) {
        case kScaleType::CENTER_MATRIX:
            scale = 1.0f;
            break;
        case kScaleType::CENTER_INSIDE:
            scale = std::min(dst.getWidth() * 1.0f / src.getWidth(),
                             dst.getHeight() * 1.0f / src.getHeight());
            break;
        case kScaleType::CENTER_CROP:
            scale = std::max(dst.getWidth() * 1.0f / src.getWidth(),
                             dst.getHeight() * 1.0f / src.getHeight());
            break;
    }
    return scale;
}

FkGraphicLayer::FkGraphicLayer() : FkGraphicEntity() {

}

FkGraphicLayer::FkGraphicLayer(const FkGraphicLayer &o)
        : FkGraphicEntity(o), id(o.id), material(o.material) {


}

FkGraphicLayer::~FkGraphicLayer() {

}

bool FkGraphicLayer::operator==(const FkGraphicLayer &obj) {
    return this->id == obj.id;
}

FkSize FkGraphicLayer::getSize() {
    FkSize value(0, 0);
    auto compo = FK_FIND_COMPO(this, FkSizeCompo);
    if (compo) {
        value = compo->size;
    }
    return value;
}

FkFloatVec3 FkGraphicLayer::getScale() {
    FkFloatVec3 value(1.0f, 1.0f, 1.0f);
    auto compo = FK_FIND_COMPO(this, FkScaleComponent);
    if (compo) {
        value = compo->value;
    }
    return value;
}

FkIntVec2 FkGraphicLayer::getTrans() {
    FkIntVec2 value(0, 0);
    auto compo = FK_FIND_COMPO(this, FkTransComponent);
    if (compo) {
        value.x = compo->value.x;
        value.y = compo->value.y;
    }
    return value;
}

FkRational FkGraphicLayer::getRotate() {
    FkRational value(0, 1);
    auto compo = FK_FIND_COMPO(this, FkRotateComponent);
    if (compo) {
        value = compo->value;
    }
    return value;
}
