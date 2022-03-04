/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-28 22:08:01
*/

#include "FkScreenQuark.h"
#include "FkRenderProto.h"
#include "FkTexDeviceQuark.h"
#include "FkTexEntity.h"
#include "FkVboCompo.h"
#include "FkRenderProgramCompo.h"
#include "FkGLDefinition.h"
#include "FkMatCompo.h"

FkScreenQuark::FkScreenQuark() : FkQuark() {
    FK_MARK_SUPER
}

FkScreenQuark::~FkScreenQuark() {

}

void FkScreenQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderProto, FkScreenQuark::_onRender);
}

FkResult FkScreenQuark::_onRender(std::shared_ptr<FkProtocol> p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkRenderProto, p);
    if (!FK_INSTANCE_OF(proto->device, FkScreenEntity)) {
        return FK_SKIP;
    }
    FK_CAST_NULLABLE_PTR_RETURN_INT(device, FkScreenEntity, proto->device);
    FK_CAST_NULLABLE_PTR_RETURN_INT(material, FkTexEntity, proto->material);
    auto srcTexCompo = material->tex();
    auto fboCompo = material->fbo();
    auto context = proto->env->getContext();
    auto size = device->size();

    auto programCompo = proto->material->findComponent<FkRenderProgramCompo>();
    auto vboCompo = proto->material->findComponent<FkVboCompo>();
    auto matCompo = proto->material->findComponent<FkMatCompo>();

    int32_t offset = 0;
    FkVertexDesc desc;
    if (FK_OK != vboCompo->getValueLoc(FkVboCompo::kValueLoc::VERTEX, offset, desc)) {
        FkLogW(FK_DEF_TAG, "Get vertex count error.");
    }

    glViewport(0, 0, size.getWidth(), size.getHeight());
    FK_GL_CHECK(programCompo->program->bind());
    vboCompo->bind();

    programCompo->program->addValue(srcTexCompo);
    programCompo->program->addValue(matCompo);
    programCompo->program->addValue(vboCompo);
    FK_GL_CHECK(glDrawArrays(GL_TRIANGLE_STRIP, 0, desc.countVertex));

    FK_GL_CHECK(programCompo->program->clear());
    programCompo->program->unbind();
    vboCompo->unbind();
    if (context) {
        FK_GL_CHECK(context->swapBuffers());
    }
    glFlush();
    return FK_OK;
}