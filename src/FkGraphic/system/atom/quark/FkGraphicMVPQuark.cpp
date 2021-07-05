/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicMVPQuark.h"
#include "FkRenderRequestPrt.h"
#include "FkMatrixComponent.h"
#include "FkMVPMatrix.h"

FkGraphicMVPQuark::FkGraphicMVPQuark() : FkQuark() {
    FK_MARK_SUPER
}

FkGraphicMVPQuark::~FkGraphicMVPQuark() {

}

void FkGraphicMVPQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderRequestPrt, FkGraphicMVPQuark::_onRenderRequest);
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
        matrix->setViewSize(1440, 2526);
        matrix->lookAt(FkFloatVec3(0.0f, 0.0f, 3.0f),
                       FkFloatVec3(0.0f, 0.0f, 0.0f),
                       FkFloatVec3(0.0f, 1.0f, 0.0f));

        auto mat = std::make_shared<FkMatrixComponent>();
        mat->value = matrix;
        layer->addComponent(mat);
    }
    return FK_OK;
}