/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-3-19 17:22:21
*/

#include "FkPointProcessQuark.h"
#include "FkGLDefinition.h"
#include "FkRenderProto.h"
#include "FkPointFCompo.h"
#include "FkTexEntity.h"
#include "FkRenderProgramCompo.h"
#include "FkVboCompo.h"
#include "FkSizeCompo.h"
#include "FkColorCompo.h"

FK_IMPL_CLASS_TYPE(FkPointProcessQuark, FkQuark)

FkPointProcessQuark::FkPointProcessQuark() : FkQuark() {

}

FkPointProcessQuark::~FkPointProcessQuark() {

}

void FkPointProcessQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderProto, FkPointProcessQuark::_onRender);
}

FkResult FkPointProcessQuark::_onRender(std::shared_ptr<FkProtocol> p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkRenderProto, p);
    if (!FK_INSTANCE_OF(proto->device, FkTexDeviceEntity)) {
        return FK_SKIP;
    }
    FK_CAST_NULLABLE_PTR_RETURN_INT(device, FkTexDeviceEntity, proto->device);
    FK_CAST_NULLABLE_PTR_RETURN_INT(materials, FkTexEntity, proto->materials);
    auto vertexCompo = FK_FIND_COMPO(materials, FkPointVertexCompo);
    if (vertexCompo == nullptr) {
        return FK_SKIP;
    }
    _drawPoints(vertexCompo, materials, device);
    return FK_FAIL;
}

//  -,- |
//------0------->
//      | +,+
FkResult FkPointProcessQuark::_drawPoints(std::shared_ptr<FkPointVertexCompo> &vertexCompo,
                                          std::shared_ptr<FkTexEntity> &materials,
                                          std::shared_ptr<FkTexDeviceEntity> &device) {
    auto fboCompo = materials->fbo();
    auto dstTexCompo = device->tex();
    auto size = device->size();
    auto programCompo = FK_FIND_COMPO(materials, FkRenderProgramCompo);
    auto vboCompo = FK_FIND_COMPO(materials, FkVboCompo);
    auto pointCompo = FK_FIND_COMPO(materials, FkPointFCompo);
    FkAssert(pointCompo != nullptr, FK_FAIL);

    int32_t offset = 0;
    FkVertexDesc desc;
    if (FK_OK != vboCompo->getValueLoc(FkVboCompo::kValueLoc::VERTEX, offset, desc)) {
        FkLogW(FK_DEF_TAG, "Get vertex count error.");
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);
    glViewport(0, 0, size.getWidth(), size.getHeight());
    fboCompo->fbo->bind();
    fboCompo->fbo->attach(dstTexCompo->tex);
    vboCompo->bind();
    FK_GL_CHECK(programCompo->program->bind());

    auto pointSizeCompo = std::make_shared<FkSizeCompo>(FkSize(pointCompo->size, pointCompo->size));
    auto colorCompo = std::make_shared<FkColorCompo>(pointCompo->color);
    programCompo->program->addValue(pointSizeCompo);
    programCompo->program->addValue(colorCompo);
    programCompo->program->addValue(vboCompo);
    FK_GL_CHECK(glDrawArrays(GL_POINTS, 0, desc.countVertex));

    fboCompo->fbo->detach(dstTexCompo->tex->desc.target);
    fboCompo->fbo->unbind();
    FK_GL_CHECK(programCompo->program->clear());
    programCompo->program->unbind();
    vboCompo->unbind();
    glDisable(GL_BLEND);
    glFlush();
    return FK_DONE;
}
