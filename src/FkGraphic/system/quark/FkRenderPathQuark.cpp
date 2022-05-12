/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2022-05-08 15:54:05
*/

#include "FkRenderPathQuark.h"
#include "FkGLDefinition.h"
#include "FkRenderProto.h"
#include "FkTexEntity.h"
#include "FkRenderProgramCompo.h"
#include "FkPathCompo.h"
#include "FkSizeCompo.h"

FK_IMPL_CLASS_TYPE(FkRenderPathQuark, FkQuark)

FkRenderPathQuark::FkRenderPathQuark() : FkQuark() {

}

FkRenderPathQuark::~FkRenderPathQuark() {

}

void FkRenderPathQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderProto, FkRenderPathQuark::_onRender);
}

FkResult FkRenderPathQuark::_onRender(std::shared_ptr<FkProtocol> &p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkRenderProto, p);
    if (!FK_INSTANCE_OF(proto->device, FkTexDeviceEntity)) {
        return FK_SKIP;
    }
    FK_CAST_NULLABLE_PTR_RETURN_INT(device, FkTexDeviceEntity, proto->device);
    FK_CAST_NULLABLE_PTR_RETURN_INT(materials, FkTexEntity, proto->materials);

    auto fboCompo = materials->fbo();
    auto dstTexCompo = device->tex();
    auto size = device->size();
    auto programCompo = FK_FIND_COMPO(materials, FkRenderProgramCompo);
    std::vector<std::shared_ptr<FkComponent>> paths;
    if (materials->findComponents(paths, FkPathCompo_Class::type) != FK_OK) {
        return FK_OK;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);
    glViewport(0, 0, size.getWidth(), size.getHeight());
    fboCompo->fbo->bind();
    fboCompo->fbo->attach(dstTexCompo->tex);
    FK_GL_CHECK(programCompo->program->bind());
    for (auto &path : paths) {
        auto compo = std::dynamic_pointer_cast<FkPathCompo>(path);
        size_t count = 0;
        if (compo && (count = compo->path->size()) > 0) {
            programCompo->program->addValue(std::make_shared<FkSizeCompo>(size));
            programCompo->program->addValue(compo);
            FK_GL_CHECK(glDrawArrays(GL_TRIANGLE_STRIP, 0, count));
        }
    }
    fboCompo->fbo->detach(dstTexCompo->tex->desc.target);
    fboCompo->fbo->unbind();
    FK_GL_CHECK(programCompo->program->clear());
    programCompo->program->unbind();
    glDisable(GL_BLEND);
    glFlush();
    return FK_DONE;
}
