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
#include "FkTransComponent.h"
#include "FkScaleComponent.h"
#include "FkRotateComponent.h"
#include "FkScaleTypeComponent.h"
#include "FkMeasureTransProto.h"

FkGraphicMVPQuark::FkGraphicMVPQuark() : FkQuark(), viewSize(1, 1) {
    FK_MARK_SUPER
}

FkGraphicMVPQuark::~FkGraphicMVPQuark() {

}

void FkGraphicMVPQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderRequestPrt, FkGraphicMVPQuark::_onRenderRequest);
    FK_PORT_DESC_QUICK_ADD(desc, FkSetSizeProto, FkGraphicMVPQuark::_onSetViewSize);
    FK_PORT_DESC_QUICK_ADD(desc, FkMeasureTransProto, FkGraphicMVPQuark::_onTransMeasure);
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
    auto proto = Fk_POINTER_CAST(FkRenderRequestPrt, p);
    std::vector<std::shared_ptr<FkGraphicComponent>> vec;
    if (FK_OK != proto->req->canvas->findComponent(vec, FkClassType::type<FkSizeComponent>())) {
        return FK_FAIL;
    }
    _calc(proto->req->canvas, viewSize, false);
    for (auto &layer : proto->req->layers) {
        _calc(layer, Fk_POINTER_CAST(FkSizeComponent, vec[0])->size, true);
    }
    return FK_OK;
}

FkResult FkGraphicMVPQuark::_onTransMeasure(std::shared_ptr<FkProtocol> p) {
    return FK_OK;
}

FkResult FkGraphicMVPQuark::_calc(std::shared_ptr<FkGraphicLayer> layer,
                                  FkSize &targetSize,
                                  bool reverseY) {
    auto matrix = std::make_shared<FkMVPMatrix>(FkMVPMatrix::kProjType::ORTHO);
    matrix->setViewSize(targetSize.getWidth(), targetSize.getHeight());
    matrix->lookAt(FkFloatVec3(0.0f, 0.0f, 1.0f),
                   FkFloatVec3(0.0f, 0.0f, 0.0f),
                   FkFloatVec3(0.0f, 1.0f, 0.0f));
    _setTranslate(matrix, layer);
    _setRotation(matrix, layer);
    _setScale(matrix, layer, targetSize, reverseY);
    matrix->calc();

    auto mat = std::make_shared<FkMatrixComponent>();
    mat->value = matrix;
    layer->addComponent(mat);
    return FK_OK;
}

FkResult FkGraphicMVPQuark::_onSetViewSize(std::shared_ptr<FkProtocol> p) {
    auto prt = Fk_POINTER_CAST(FkSetSizeProto, p);
    viewSize = prt->value;
    return FK_OK;
}

float FkGraphicMVPQuark::_getViewScale(std::shared_ptr<FkGraphicLayer> layer, FkSize &targetSize) {
    float scale = 1.0f;
    auto scaleType = layer->findComponent<FkScaleTypeComponent>();
    FkAssert(nullptr != scaleType, scale);
    auto size = layer->findComponent<FkSizeComponent>();
    FkAssert(nullptr != size, scale);
    auto &layerSize = size->size;
    switch (scaleType->value) {
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

FkResult FkGraphicMVPQuark::_setRotation(std::shared_ptr<FkMVPMatrix> matrix,
                                         std::shared_ptr<FkGraphicLayer> layer) {
    auto comp = layer->findComponent<FkRotateComponent>();
    FkAssert(nullptr != comp, FK_FAIL);
    matrix->setRotation(comp->value);
    return FK_OK;
}

FkResult FkGraphicMVPQuark::_setScale(std::shared_ptr<FkMVPMatrix> matrix,
                                      std::shared_ptr<FkGraphicLayer> layer,
                                      FkSize &targetSize,
                                      bool reverseY) {
    auto comp = layer->findComponent<FkScaleComponent>();
    FkAssert(nullptr != comp, FK_FAIL);
    auto scaleOfType = _getViewScale(layer, targetSize);
    FkFloatVec3 scale(comp->value.x * scaleOfType,
                      comp->value.y * (reverseY ? -1.0f : 1.0f) * scaleOfType, comp->value.z);
    matrix->setScale(scale);
    return FK_OK;
}

FkResult FkGraphicMVPQuark::_setTranslate(std::shared_ptr<FkMVPMatrix> matrix,
                                          std::shared_ptr<FkGraphicLayer> layer) {
    auto comp = layer->findComponent<FkTransComponent>();
    FkAssert(nullptr != comp, FK_FAIL);
    FkFloatVec3 trans(comp->value.x, comp->value.y, 0.0f);
    matrix->setTranslate(trans);
    return FK_OK;
}
