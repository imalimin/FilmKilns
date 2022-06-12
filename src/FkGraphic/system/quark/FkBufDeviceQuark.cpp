/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-26 12:42:33
*/

#include "FkBufDeviceQuark.h"
#include "FkRenderProto.h"
#include "FkTexEntity.h"
#include "FkGLDefinition.h"

FK_IMPL_CLASS_TYPE(FkBufDeviceQuark, FkQuark)

FkBufDeviceQuark::FkBufDeviceQuark() : FkQuark() {

}

FkBufDeviceQuark::~FkBufDeviceQuark() {
}

void FkBufDeviceQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderProto, FkBufDeviceQuark::_onRender);
}

FkResult FkBufDeviceQuark::onCreate() {
    auto ret = FkQuark::onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkBufDeviceQuark::onDestroy() {
    auto ret = FkQuark::onDestroy();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkBufDeviceQuark::onStart() {
    auto ret = FkQuark::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkBufDeviceQuark::onStop() {
    return FkQuark::onStop();
}

FkResult FkBufDeviceQuark::_onRender(std::shared_ptr<FkProtocol> p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkRenderProto, p);
    if (!FK_INSTANCE_OF(proto->device, FkBufDeviceEntity)) {
        return FK_SKIP;
    }
    FK_CAST_NULLABLE_PTR_RETURN_INT(device, FkBufDeviceEntity, proto->device);
    FK_CAST_NULLABLE_PTR_RETURN_INT(material, FkTexEntity, proto->materials);
    auto fboCompo = material->fbo();
    if (fboCompo == nullptr) {
        return FK_NPE;
    }
    auto texArray = material->texArray();
    if (texArray->empty()) {
        FkLogW(FK_DEF_TAG, "Not support for multi-texture frame buffer object.");
        return FK_SKIP;
    }
    auto texSize = material->size();
    auto tex = (*texArray)[0];
    glFinish();
    fboCompo->fbo->bind();
    fboCompo->fbo->attach(tex);
    glViewport(0, 0, texSize.getWidth(), texSize.getHeight());

    auto dstSize = device->getSize();
    if (dstSize.isZero()) {
        dstSize = texSize;
    }
    auto pos = device->getPosition();
    auto buf = device->buffer()->data();
    glReadPixels(pos.x, pos.y,
                 dstSize.getWidth(), dstSize.getHeight(),
                 GL_RGBA, GL_UNSIGNED_BYTE, buf);
    fboCompo->fbo->detach(tex->desc.target);
    fboCompo->fbo->unbind();
    return FK_OK;
}