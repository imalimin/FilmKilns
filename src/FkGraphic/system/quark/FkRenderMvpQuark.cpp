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
#include "FkRotateComponent.h"
#include "FkScaleComponent.h"
#include "FkTransComponent.h"

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
    FkSize targetSize;
    if (FK_INSTANCE_OF(proto->device, FkTexDeviceEntity)) {
        FK_CAST_NULLABLE_PTR_RETURN_INT(device, FkTexDeviceEntity, proto->device);
        targetSize = device->size();
    } else if (FK_INSTANCE_OF(proto->device, FkScreenEntity)) {
        FK_CAST_NULLABLE_PTR_RETURN_INT(device, FkScreenEntity, proto->device);
        targetSize = device->size();
    } else {
        return FK_SKIP;
    }
    auto matrix = _calcMatrix(proto->trans, targetSize, false);
    if (matrix) {
        auto mat = std::make_shared<FkMatCompo>();
        mat->value = matrix;
        proto->material->addComponent(mat);
    }
    return FK_OK;
}

std::shared_ptr<FkMVPMatrix> FkRenderMvpQuark::_calcMatrix(
        std::shared_ptr<FkTransEntity> transEntity,
        FkSize &targetSize,
        bool reverseY) {
    auto matrix = std::make_shared<FkMVPMatrix>(FkMVPMatrix::kProjType::ORTHO);
    matrix->setViewSize(targetSize.getWidth(), targetSize.getHeight());
    matrix->lookAt(FkFloatVec3(0.0f, 0.0f, 1.0f),
                   FkFloatVec3(0.0f, 0.0f, 0.0f),
                   FkFloatVec3(0.0f, 1.0f, 0.0f));
    if (transEntity) {
        _setTranslate(matrix, transEntity);
        _setRotation(matrix, transEntity);
        _setScale(matrix, transEntity, targetSize, reverseY);
    }
    matrix->calc();
    return matrix;
}

FkResult FkRenderMvpQuark::_setRotation(std::shared_ptr<FkMVPMatrix> &matrix,
                                        std::shared_ptr<FkTransEntity> &transEntity) {
    auto compo = transEntity->findComponent<FkRotateComponent>();
    if (compo) {
        matrix->setRotation(compo->value);
    }
    return FK_OK;
}

FkResult FkRenderMvpQuark::_setScale(std::shared_ptr<FkMVPMatrix> &matrix,
                                     std::shared_ptr<FkTransEntity> &transEntity,
                                     FkSize &targetSize,
                                     bool reverseY) {
    auto compo = transEntity->findComponent<FkScaleComponent>();
    if (compo) {
        FkFloatVec3 scale(compo->value.x,
                          compo->value.y * (reverseY ? -1.0f : 1.0f),
                          compo->value.z);
        matrix->setScale(scale);
    }
    return FK_OK;
}

FkResult FkRenderMvpQuark::_setTranslate(std::shared_ptr<FkMVPMatrix> &matrix,
                                         std::shared_ptr<FkTransEntity> &transEntity) {
    auto compo = transEntity->findComponent<FkTransComponent>();
    if (compo) {
        FkFloatVec3 trans(compo->value.x, compo->value.y, 0.0f);
        matrix->setTranslate(trans);
    }
    return FK_OK;
}
