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

float FkGraphicLayer::calcScaleWithScaleType(std::shared_ptr<FkGraphicLayer> layer,
                                             kScaleType scaleType,
                                             FkSize &targetSize) {
    float scale = 1.0f;
    auto size = layer->findComponent<FkSizeCompo>();
    FkAssert(nullptr != size, scale);
    auto &layerSize = size->size;
    switch (scaleType) {
        case kScaleType::CENTER_MATRIX:
            scale = 1.0f;
            break;
        case kScaleType::CENTER_INSIDE:
            scale = std::min(targetSize.getWidth() * 1.0f / layerSize.getWidth(),
                             targetSize.getHeight() * 1.0f / layerSize.getHeight());
            break;
        case kScaleType::CENTER_CROP:
            scale = std::max(targetSize.getWidth() * 1.0f / layerSize.getWidth(),
                             targetSize.getHeight() * 1.0f / layerSize.getHeight());
            break;
    }
    return scale;
}

FkGraphicLayer::FkGraphicLayer() : FkGraphicEntity() {
    FK_MARK_SUPER
}

FkGraphicLayer::FkGraphicLayer(const FkGraphicLayer &o)
        : FkGraphicEntity(o), id(o.id), material(o.material) {
    FK_MARK_SUPER

}

FkGraphicLayer::~FkGraphicLayer() {

}

bool FkGraphicLayer::operator==(const FkGraphicLayer &obj) {
    return this->id == obj.id;
}

FkSize FkGraphicLayer::getSize() {
    FkSize value(0, 0);
    auto compo = findComponent<FkSizeCompo>();
    if (compo) {
        value = compo->size;
    }
    return value;
}

FkFloatVec3 FkGraphicLayer::getScale() {
    FkFloatVec3 value(1.0f, 1.0f, 1.0f);
    auto compo = findComponent<FkScaleComponent>();
    if (compo) {
        value = compo->value;
    }
    return value;
}

FkIntVec2 FkGraphicLayer::getTrans() {
    FkIntVec2 value(0, 0);
    auto compo = findComponent<FkTransComponent>();
    if (compo) {
        value = compo->value;
    }
    return value;
}

FkRational FkGraphicLayer::getRotate() {
    FkRational value(0, 1);
    auto compo = findComponent<FkRotateComponent>();
    if (compo) {
        value = compo->value;
    }
    return value;
}
