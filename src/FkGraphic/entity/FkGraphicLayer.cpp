/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicLayer.h"
#include "FkSizeComponent.h"

float FkGraphicLayer::calcScaleWithScaleType(std::shared_ptr<FkGraphicLayer> layer,
                                             kScaleType scaleType,
                                             FkSize &targetSize) {
    float scale = 1.0f;
    auto size = layer->findComponent<FkSizeComponent>();
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

FkGraphicLayer::FkGraphicLayer(const FkGraphicLayer &o) : FkGraphicEntity(o), id(o.id) {
    FK_MARK_SUPER

}

FkGraphicLayer::~FkGraphicLayer() {

}
