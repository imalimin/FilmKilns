/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicMVPQuark.h"
#include "FkRenderRequestPrt.h"
#include "FkMatrixComponent.h"
#include "FkSetSizeProto.h"
#include "FkSizeComponent.h"

FkGraphicMVPQuark::FkGraphicMVPQuark() : FkQuark(), viewSize(1, 1) {
    FK_MARK_SUPER
}

FkGraphicMVPQuark::~FkGraphicMVPQuark() {

}

void FkGraphicMVPQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderRequestPrt, FkGraphicMVPQuark::_onRenderRequest);
    FK_PORT_DESC_QUICK_ADD(desc, FkSetSizeProto, FkGraphicMVPQuark::_onSetViewSize);
}

FkResult FkGraphicMVPQuark::onCreate() {
    return FkQuark::onCreate();
}

FkResult FkGraphicMVPQuark::onDestroy() {
    return FkQuark::onDestroy();
}

FkResult FkGraphicMVPQuark::onStart() {
    return FkQuark::onStart();
}

FkResult FkGraphicMVPQuark::onStop() {
    return FkQuark::onStop();
}

FkResult FkGraphicMVPQuark::_onRenderRequest(std::shared_ptr<FkProtocol> p) {
    auto prt = Fk_POINTER_CAST(FkRenderRequestPrt, p);
    for (auto &layer : prt->req->layers) {
        auto matrix = std::make_shared<FkMVPMatrix>(FkMVPMatrix::kProjType::ORTHO);
        matrix->setViewSize(viewSize.getWidth(), viewSize.getHeight());
        matrix->lookAt(FkFloatVec3(0.0f, 0.0f, 1.0f),
                       FkFloatVec3(0.0f, 0.0f, 0.0f),
                       FkFloatVec3(0.0f, 1.0f, 0.0f));
        _setScale(matrix, layer);
        _setRotation(matrix, layer);
        _setTranslate(matrix, layer);
        matrix->calc();

        auto mat = std::make_shared<FkMatrixComponent>();
        mat->value = matrix;
        layer->addComponent(mat);
    }
    return FK_OK;
}

FkResult FkGraphicMVPQuark::_onSetViewSize(std::shared_ptr<FkProtocol> p) {
    auto prt = Fk_POINTER_CAST(FkSetSizeProto, p);
    viewSize = prt->value;
    return FK_OK;
}

float FkGraphicMVPQuark::_getViewScale(std::shared_ptr<FkGraphicLayer> layer) {
    float scale = 1.0f;
    std::vector<std::shared_ptr<FkGraphicComponent>> vec;
    std::shared_ptr<FkSizeComponent> size = nullptr;
    if (FK_OK != layer->findComponent(vec, FkClassType::type<FkSizeComponent>())) {
        return scale;
    }
    size = Fk_POINTER_CAST(FkSizeComponent, vec[0]);
    auto &layerSize = size->size;
    switch (scaleType) {
        case kScaleType::CENTER_MATRIX:
            scale = 1.0f;
            break;
        case kScaleType::CENTER_INSIDE:
            scale = std::min(viewSize.getWidth() * 1.0f / layerSize.getWidth(), viewSize.getHeight() * 1.0f / layerSize.getHeight());
            break;
        case kScaleType::CENTER_CROP:
            scale = std::max(viewSize.getWidth() * 1.0f / layerSize.getWidth(), viewSize.getHeight() * 1.0f / layerSize.getHeight());
            break;
    }
    return scale;
}

FkResult FkGraphicMVPQuark::_setRotation(std::shared_ptr<FkMVPMatrix> matrix,
                                         std::shared_ptr<FkGraphicLayer> layer) {
    return FK_OK;
}

FkResult FkGraphicMVPQuark::_setScale(std::shared_ptr<FkMVPMatrix> matrix,
                                      std::shared_ptr<FkGraphicLayer> layer) {
    auto scaleOfType = _getViewScale(layer);
    FkFloatVec3 scale(1.0f * scaleOfType, -1.0f * scaleOfType, 1.0f);
    matrix->setScale(scale);
    return FK_OK;
}

FkResult FkGraphicMVPQuark::_setTranslate(std::shared_ptr<FkMVPMatrix> matrix,
                                          std::shared_ptr<FkGraphicLayer> layer) {
    return FK_OK;
}
