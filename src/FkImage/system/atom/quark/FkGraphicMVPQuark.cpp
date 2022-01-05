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
#include "ext.hpp"

FkGraphicMVPQuark::FkGraphicMVPQuark() : FkQuark() {
    FK_MARK_SUPER
}

FkGraphicMVPQuark::~FkGraphicMVPQuark() {

}

void FkGraphicMVPQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderRequestPrt, FkGraphicMVPQuark::_onRenderRequest);
    FK_PORT_DESC_QUICK_ADD(desc, FkMeasureTransProto, FkGraphicMVPQuark::_onMeasureTrans);
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
    auto canvas = proto->req->getCanvas();
    FkAssert(nullptr != canvas, FK_FAIL);
    auto sizeComp = canvas->findComponent<FkSizeComponent>();
    FkAssert(nullptr != sizeComp, FK_FAIL);
    _calc(canvas, proto->winSize, false);
    for (auto &layer : proto->req->layers) {
        if (Fk_CANVAS_ID == layer->id) {
            continue;
        }
        _calc(layer, sizeComp->size, true);
    }
    return FK_OK;
}

FkResult FkGraphicMVPQuark::_onMeasureTrans(std::shared_ptr<FkProtocol> p) {
    auto proto = Fk_POINTER_CAST(FkMeasureTransProto, p);
    auto canvasScale = proto->canvas->findComponent<FkScaleComponent>();
    FkAssert(nullptr != canvasScale, FK_FAIL);
    auto layerScale = proto->layer->findComponent<FkScaleComponent>();
    FkAssert(nullptr != layerScale, FK_FAIL);
    auto layerRotate = proto->layer->findComponent<FkRotateComponent>();
    FkAssert(nullptr != layerScale, FK_FAIL);

    glm::mat4 mat = glm::mat4(1.0f);
    mat = glm::rotate(mat, -layerRotate->value.num * 1.0f / layerRotate->value.den,
                      glm::vec3(0.0f, 0.0f, 1.0f));
    mat = glm::scale(mat, glm::vec3(1.0f / canvasScale->value.x / layerScale->value.x,
                                    1.0f / canvasScale->value.x / layerScale->value.y, 1.0f));
    glm::vec4 vec(proto->value.x, proto->value.y, 0, 0);
    vec = vec * mat;
    proto->value.x = vec.x;
    proto->value.y = vec.y;
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
    FkFloatVec3 scale(comp->value.x,
                      comp->value.y * (reverseY ? -1.0f : 1.0f), comp->value.z);
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
