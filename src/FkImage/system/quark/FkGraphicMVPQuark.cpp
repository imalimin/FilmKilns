/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicMVPQuark.h"
#include "FkRenderRequestPrt.h"
#include "FkMatCompo.h"
#include "FkSizeCompo.h"
#include "FkTransComponent.h"
#include "FkScaleComponent.h"
#include "FkRotateComponent.h"
#include "FkScaleTypeComponent.h"
#include "FkMeasureTransProto.h"
#include "FkMeasurePointProto.h"
#include "ext.hpp"

FK_IMPL_CLASS_TYPE(FkGraphicMVPQuark, FkQuark)

FkGraphicMVPQuark::FkGraphicMVPQuark() : FkQuark() {

}

FkGraphicMVPQuark::~FkGraphicMVPQuark() {

}

void FkGraphicMVPQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderRequestPrt, FkGraphicMVPQuark::_onRenderRequest);
    FK_PORT_DESC_QUICK_ADD(desc, FkMeasureTransProto, FkGraphicMVPQuark::_onMeasureTrans);
    FK_PORT_DESC_QUICK_ADD(desc, FkMeasurePointProto, FkGraphicMVPQuark::_onMeasurePoint);
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
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkRenderRequestPrt, p);
    auto canvas = proto->req->getCanvas();
    FkAssert(nullptr != canvas, FK_FAIL);
    auto sizeComp = FK_FIND_COMPO(canvas, FkSizeCompo);
    FkAssert(nullptr != sizeComp, FK_FAIL);
    _calc(canvas, proto->winSize, true);
    for (auto &layer : proto->req->layers) {
        if (Fk_CANVAS_ID == layer->id) {
            continue;
        }
        _calc(layer, sizeComp->size, false);
    }
    return FK_OK;
}

FkResult FkGraphicMVPQuark::_onMeasureTrans(std::shared_ptr<FkProtocol> p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkMeasureTransProto, p);
    auto canvasScale = FK_FIND_COMPO(proto->canvas, FkScaleComponent);
    FkAssert(nullptr != canvasScale, FK_FAIL);
    canvasScale = std::make_shared<FkScaleComponent>(canvasScale->value);
    auto canvasRotate = FK_FIND_COMPO(proto->canvas, FkRotateComponent);
    FkAssert(nullptr != canvasRotate, FK_FAIL);
    glm::mat4 canvasMat = glm::mat4(1.0f);
    canvasMat = glm::rotate(canvasMat, -canvasRotate->value.toFloat(), glm::vec3(0.0f, 0.0f, 1.0f));
    canvasMat = glm::scale(canvasMat, glm::vec3(1.0f / canvasScale->value.x,
                                                1.0f / canvasScale->value.x, 1.0f));

    std::shared_ptr<FkScaleComponent> layerScale = nullptr;
    std::shared_ptr<FkRotateComponent> layerRotate = nullptr;
    if (proto->layer) {
        layerScale = FK_FIND_COMPO(proto->layer, FkScaleComponent);
        layerRotate = FK_FIND_COMPO(proto->layer, FkRotateComponent);
    }
    if (layerScale == nullptr) {
        layerScale = std::make_shared<FkScaleComponent>();
        layerScale->value = FkFloatVec3(1.0f, 1.0f, 1.0f);
    }
    if (layerRotate == nullptr) {
        layerRotate = std::make_shared<FkRotateComponent>();
        layerRotate->value = FkRational(0, 1);
    }
    glm::mat4 layerMat = glm::mat4(1.0f);
    layerMat = glm::rotate(layerMat, -layerRotate->value.toFloat(), glm::vec3(0.0f, 0.0f, 1.0f));
    layerMat = glm::scale(layerMat, glm::vec3(1.0f / layerScale->value.x,
                                              1.0f / layerScale->value.x, 1.0f));

    glm::vec4 vec(proto->value.x, proto->value.y, 0, 0);
    vec = vec * canvasMat * layerMat;
    proto->value.x = vec.x;
    proto->value.y = vec.y;
    return FK_OK;
}

FkResult FkGraphicMVPQuark::_onMeasurePoint(std::shared_ptr<FkProtocol> p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkMeasurePointProto, p);
    auto winSize = proto->winSize;
    proto->value = FkDoubleVec2(proto->value.x - winSize.getWidth() / 2.0f,
                             proto->value.y - winSize.getHeight() / 2.0f);
    proto->value = _calcPoint2OtherCoordination(proto->value, proto->canvas);
    if (proto->layer) {
        proto->value = _calcPoint2OtherCoordination(proto->value, proto->layer);
    }
    return FK_OK;
}

FkDoubleVec2 FkGraphicMVPQuark::_calcPoint2OtherCoordination(FkDoubleVec2 &point,
                                                          std::shared_ptr<FkGraphicLayer> &layer) {
    auto trans = layer->getTrans();
    auto rotate = layer->getRotate();
    auto scale = layer->getScale();
    auto size = layer->getSize();

    auto mat = std::make_shared<FkMVPMatrix>(FkMVPMatrix::kProjType::ORTHO);
    mat->setViewSize(size.getWidth(), size.getHeight());
    mat->lookAt(FkFloatVec3(0.0f, 0.0f, 1.0f),
                FkFloatVec3(0.0f, 0.0f, 0.0f),
                FkFloatVec3(0.0f, 1.0f, 0.0f));
    mat->setScale(FkFloatVec3(1.0f / scale.x, 1.0f / scale.y, 1.0f));

    auto fixMat = std::make_shared<FkMVPMatrix>(FkMVPMatrix::kProjType::ORTHO);
    fixMat->setRotation(rotate);

    point = fixMat->calcVec2(point);
    point = mat->calcVec2(point);
    return FkDoubleVec2(point.x * size.getWidth() / 2.0f - trans.x,
                        point.y * size.getHeight() / 2.0f - trans.y);
}

FkResult FkGraphicMVPQuark::_calc(std::shared_ptr<FkGraphicLayer> layer,
                                  FkSize &targetSize,
                                  bool reverseY) {
    auto compo = std::make_shared<FkMatCompo>();
    compo->value = _calcMat(layer, targetSize, reverseY);
    layer->addComponent(compo);
    return FK_OK;
}

std::shared_ptr<FkMVPMatrix> FkGraphicMVPQuark::_calcMat(std::shared_ptr<FkGraphicLayer> layer,
                                                         FkSize &targetSize,
                                                         bool reverseY) {
    auto matrix = std::make_shared<FkMVPMatrix>(FkMVPMatrix::kProjType::ORTHO);
    matrix->setViewSize(targetSize.getWidth(), targetSize.getHeight());
    matrix->lookAt(FkFloatVec3(0.0f, 0.0f, 1.0f),
                   FkFloatVec3(0.0f, 0.0f, 0.0f),
                   FkFloatVec3(0.0f, 1.0f, 0.0f));
    _setTranslate(matrix, layer);
    _setRotation(matrix, layer, reverseY);
    _setScale(matrix, layer, targetSize, reverseY);
    matrix->calc();
    return matrix;
}

FkResult FkGraphicMVPQuark::_setRotation(std::shared_ptr<FkMVPMatrix> matrix,
                                         std::shared_ptr<FkGraphicLayer> layer,
                                         bool reverseY) {
    auto comp = FK_FIND_COMPO(layer, FkRotateComponent);
    FkAssert(nullptr != comp, FK_FAIL);
    auto value = comp->value;
    if (reverseY) {
        value.num *= -1;
    }
    matrix->setRotation(value);
    return FK_OK;
}

FkResult FkGraphicMVPQuark::_setScale(std::shared_ptr<FkMVPMatrix> matrix,
                                      std::shared_ptr<FkGraphicLayer> layer,
                                      FkSize &targetSize,
                                      bool reverseY) {
    auto comp = FK_FIND_COMPO(layer, FkScaleComponent);
    FkAssert(nullptr != comp, FK_FAIL);
    FkFloatVec3 scale(comp->value.x,
                      comp->value.y * (reverseY ? -1.0f : 1.0f), comp->value.z);
    matrix->setScale(scale);
    return FK_OK;
}

FkResult FkGraphicMVPQuark::_setTranslate(std::shared_ptr<FkMVPMatrix> matrix,
                                          std::shared_ptr<FkGraphicLayer> layer) {
    auto comp = FK_FIND_COMPO(layer, FkTransComponent);
    FkAssert(nullptr != comp, FK_FAIL);
    FkFloatVec3 trans(comp->value.x, comp->value.y, 0.0f);
    matrix->setTranslate(trans);
    return FK_OK;
}
