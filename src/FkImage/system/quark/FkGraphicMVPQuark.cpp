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

FkGraphicMVPQuark::FkGraphicMVPQuark() : FkQuark() {
    FK_MARK_SUPER
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
    auto sizeComp = canvas->findComponent<FkSizeCompo>();
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
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkMeasureTransProto, p);
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
FkResult FkGraphicMVPQuark::_onMeasurePoint(std::shared_ptr<FkProtocol> p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkMeasurePointProto, p);
    auto winSize = proto->winSize;
    auto layerSize = proto->layer->getSize();
    auto canvasSize = proto->canvas->getSize();

    auto matrix = std::make_shared<FkMVPMatrix>(FkMVPMatrix::kProjType::ORTHO);
    matrix->setViewSize(canvasSize.getWidth(), canvasSize.getHeight());
    matrix->lookAt(FkFloatVec3(0.0f, 0.0f, 1.0f),
                   FkFloatVec3(0.0f, 0.0f, 0.0f),
                   FkFloatVec3(0.0f, 1.0f, 0.0f));
    auto trans = proto->canvas->getTrans();
    auto rotate = proto->canvas->getRotate();
    auto scale = proto->canvas->getScale();
//    matrix->setTranslate(FkFloatVec3(-trans.x, -trans.y, 0.0f));
    matrix->setRotation(FkRational(-rotate.num, rotate.den));
    matrix->setScale(FkFloatVec3(1.0f / scale.x, 1.0f / scale.y, 1.0f));
    matrix->calc();
    glm::vec4 vec0(proto->value.x - winSize.getWidth() / 2.0f - trans.x,
                  proto->value.y - winSize.getHeight() / 2.0f - trans.y, 0, 1.0f);
    vec0 = vec0 * matrix->mat4;
    FkLogI("aliminabcd", "%f, %f", vec0.x, vec0.y);
    auto *data = (float *) matrix->get();
    for (int i = 0; i < 4; ++i) {
        FkLogI("aliminabcd", "[%f, %f, %f, %f]", data[i * 4 + 0], data[i * 4 + 1], data[i * 4 + 2], data[i * 4 + 3]);
    }
    FkIntVec2 pos(vec0.x * canvasSize.getWidth() / 2.0f, vec0.y * canvasSize.getHeight() / 2.0f);

    auto layerMat = std::make_shared<FkMVPMatrix>(FkMVPMatrix::kProjType::ORTHO);
    layerMat->setViewSize(layerSize.getWidth(), layerSize.getHeight());
    layerMat->lookAt(FkFloatVec3(0.0f, 0.0f, 1.0f),
                   FkFloatVec3(0.0f, 0.0f, 0.0f),
                   FkFloatVec3(0.0f, 1.0f, 0.0f));
    trans = proto->layer->getTrans();
    rotate = proto->layer->getRotate();
    scale = proto->layer->getScale();
//    layerMat->setTranslate(FkFloatVec3(-trans.x, trans.y, 0.0f));
    layerMat->setRotation(FkRational(-rotate.num, rotate.den));
    layerMat->setScale(FkFloatVec3(1.0f / scale.x, 1.0f / scale.y, 1.0f));
    layerMat->calc();
    glm::vec4 vec1(pos.x - trans.x, pos.y - trans.y, 0, 1.0f);
    vec1 = vec1 * layerMat->mat4;
    FkLogI("aliminabcd", "%f, %f", vec1.x, vec1.y);
    auto *data1 = (float *) layerMat->get();
    for (int i = 0; i < 4; ++i) {
        FkLogI("aliminabcd", "[%f, %f, %f, %f]", data1[i * 4 + 0], data1[i * 4 + 1], data1[i * 4 + 2], data1[i * 4 + 3]);
    }
    proto->value = FkIntVec2((vec1.x + 1.0f) * layerSize.getWidth() / 2.0f, (vec1.y + 1.0f) * layerSize.getHeight() / 2.0f);
    FkLogI("aliminabcd", "%d, %d", proto->value.x, proto->value.y);

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

    auto mat = std::make_shared<FkMatCompo>();
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
