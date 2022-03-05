/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-27 12:55:44
*/

#include "FkTexDeviceQuark.h"
#include "FkRenderProto.h"
#include "FkTexDeviceQuark.h"
#include "FkTexEntity.h"
#include "FkVboCompo.h"
#include "FkRenderProgramCompo.h"
#include "FkGLDefinition.h"
#include "FkMatCompo.h"

FkTexDeviceQuark::FkTexDeviceQuark() : FkQuark() {
    FK_MARK_SUPER
}

FkTexDeviceQuark::~FkTexDeviceQuark() {

}

void FkTexDeviceQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderProto, FkTexDeviceQuark::_onRender);
}

FkResult FkTexDeviceQuark::onCreate() {
    auto ret = FkQuark::onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkTexDeviceQuark::onDestroy() {
    auto ret = FkQuark::onDestroy();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkTexDeviceQuark::onStart() {
    auto ret = FkQuark::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkTexDeviceQuark::onStop() {
    auto ret = FkQuark::onStop();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkTexDeviceQuark::_onRender(std::shared_ptr<FkProtocol> p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkRenderProto, p);
    if (!FK_INSTANCE_OF(proto->device, FkTexDeviceEntity)) {
        return FK_SKIP;
    }
    FK_CAST_NULLABLE_PTR_RETURN_INT(device, FkTexDeviceEntity, proto->device);
    FK_CAST_NULLABLE_PTR_RETURN_INT(material, FkTexEntity, proto->material);
    auto srcTexCompo = material->tex();
    auto fboCompo = material->fbo();
    auto dstTexCompo = device->tex();
    auto size = device->size();

    auto programCompo = proto->material->findComponent<FkRenderProgramCompo>();
    auto vboCompo = proto->material->findComponent<FkVboCompo>();
    auto matCompo = proto->material->findComponent<FkMatCompo>();

    int32_t offset = 0;
    FkVertexDesc desc;
    if (FK_OK != vboCompo->getValueLoc(FkVboCompo::kValueLoc::VERTEX, offset, desc)) {
        FkLogW(FK_DEF_TAG, "Get vertex count error.");
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);
    glViewport(0, 0, size.getWidth(), size.getHeight());
    fboCompo->fbo->attach(dstTexCompo->tex, true);
    FK_GL_CHECK(programCompo->program->bind());
    vboCompo->bind();

    programCompo->program->addValue(srcTexCompo);
    programCompo->program->addValue(matCompo);
    programCompo->program->addValue(vboCompo);
    FK_GL_CHECK(glDrawArrays(GL_TRIANGLE_STRIP, 0, desc.countVertex));

    fboCompo->fbo->detach(dstTexCompo->tex->desc.target);
    FK_GL_CHECK(programCompo->program->clear());
    programCompo->program->unbind();
    vboCompo->unbind();
    glDisable(GL_BLEND);
    glFlush();
    return FK_OK;
}