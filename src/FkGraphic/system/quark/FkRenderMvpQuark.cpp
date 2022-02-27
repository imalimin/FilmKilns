/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-27 14:59:49
*/

#include "FkRenderMvpQuark.h"
#include "FkRenderProto.h"
#include "FkMatCompo.h"

FkRenderMvpQuark::FkRenderMvpQuark() : FkQuark() {
    FK_MARK_SUPER
}

FkRenderMvpQuark::~FkRenderMvpQuark() {

}

void FkRenderMvpQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderProto, FkRenderMvpQuark::_onRender);
}

FkResult FkRenderMvpQuark::onCreate() {
    return FkQuark::onCreate();
}

FkResult FkRenderMvpQuark::onDestroy() {
    return FkQuark::onDestroy();
}

FkResult FkRenderMvpQuark::onStart() {
    auto ret = FkQuark::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    return FkQuark::onStart();
}

FkResult FkRenderMvpQuark::onStop() {
    return FkQuark::onStop();
}

FkResult FkRenderMvpQuark::_onRender(std::shared_ptr<FkProtocol> &p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkRenderProto, p);
    if (!FK_INSTANCE_OF(proto->device, FkTexDeviceEntity)) {
        return FK_SKIP;
    }
    FK_CAST_NULLABLE_PTR_RETURN_INT(device, FkTexDeviceEntity, proto->device);
    auto targetSize = device->size();
    _calc(proto->material, targetSize, false);
    return FK_OK;
}

FkResult FkRenderMvpQuark::_calc(std::shared_ptr<FkMaterialEntity> &material,
                                  FkSize &targetSize,
                                  bool reverseY) {
    auto matrix = std::make_shared<FkMVPMatrix>(FkMVPMatrix::kProjType::ORTHO);
    matrix->setViewSize(targetSize.getWidth(), targetSize.getHeight());
    matrix->lookAt(FkFloatVec3(0.0f, 0.0f, 1.0f),
                   FkFloatVec3(0.0f, 0.0f, 0.0f),
                   FkFloatVec3(0.0f, 1.0f, 0.0f));
    _setTranslate(matrix, material);
    _setRotation(matrix, material);
    _setScale(matrix, material, targetSize, reverseY);
    matrix->calc();

    auto mat = std::make_shared<FkMatCompo>();
    mat->value = matrix;
    material->addComponent(mat);
    return FK_OK;
}

FkResult FkRenderMvpQuark::_setRotation(std::shared_ptr<FkMVPMatrix> matrix,
                                        std::shared_ptr<FkMaterialEntity> &material) {
//    auto comp = layer->findComponent<FkRotateComponent>();
//    FkAssert(nullptr != comp, FK_FAIL);
    FkRational value(0, 1);
    matrix->setRotation(value);
    return FK_OK;
}

FkResult FkRenderMvpQuark::_setScale(std::shared_ptr<FkMVPMatrix> matrix,
                                     std::shared_ptr<FkMaterialEntity> &material,
                                      FkSize &targetSize,
                                      bool reverseY) {
//    auto comp = layer->findComponent<FkScaleComponent>();
//    FkAssert(nullptr != comp, FK_FAIL);
    FkFloatVec3 scale(1.0f, 1.0f, 1.0f);
    matrix->setScale(scale);
    return FK_OK;
}

FkResult FkRenderMvpQuark::_setTranslate(std::shared_ptr<FkMVPMatrix> matrix,
                                         std::shared_ptr<FkMaterialEntity> &material) {
//    auto comp = layer->findComponent<FkTransComponent>();
//    FkAssert(nullptr != comp, FK_FAIL);
    FkFloatVec3 trans(0.0f, 0.0f, 0.0f);
    matrix->setTranslate(trans);
    return FK_OK;
}
