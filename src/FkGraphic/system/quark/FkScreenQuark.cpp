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

FkResult FkScreenQuark::onStart() {
    auto ret = FkQuark::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    allocator = std::make_shared<FkGraphicProgramAllocator>(10);
    FkProgramDescription desc(FkProgramDescription::kType::CANVAS_MOSAIC);
    program = allocator->alloc(desc);
    texAllocator = std::make_shared<FkGraphicAllocator>(1080 * 1920 * 4 * 10);

    FkTexDescription tdesc(GL_TEXTURE_2D);
    tdesc.fmt = FkColor::kFormat::RGBA;
    tdesc.size = FkSize(32, 32);
    tex = texAllocator->alloc(tdesc);

    uint8_t pixels[tdesc.size.getWidth() * tdesc.size.getHeight() * 4];
    for (int h = 0; h < tdesc.size.getHeight(); ++h) {
        for (int w = 0; w < tdesc.size.getWidth(); ++w) {
            auto index = tdesc.size.getWidth() * h + w;
            pixels[index] = 255;
            pixels[index + 1] = 255;
            pixels[index + 2] = 255;
            pixels[index + 3] = 255;
        }
    }
    tex->update(pixels);
    return FkQuark::onStart();
}

FkResult FkScreenQuark::onStop() {
    tex = nullptr;
    texAllocator->release();
    texAllocator = nullptr;
    program = nullptr;
    allocator->release();
    allocator = nullptr;
    return FkQuark::onStop();
}

FkResult FkScreenQuark::_onRender(std::shared_ptr<FkProtocol> &p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkRenderProto, p);
    if (!FK_INSTANCE_OF(proto->device, FkScreenEntity)) {
        return FK_SKIP;
    }
    FK_CAST_NULLABLE_PTR_RETURN_INT(device, FkScreenEntity, proto->device);
    FK_CAST_NULLABLE_PTR_RETURN_INT(material, FkTexEntity, proto->materials);
    auto srcTexCompo = material->tex();
    auto fboCompo = material->fbo();
    auto context = proto->env->getContext();
    auto size = device->size();

    auto programCompo = proto->materials->findComponent<FkRenderProgramCompo>();
    auto vboCompo = proto->materials->findComponent<FkVboCompo>();
    auto matCompo = proto->materials->findComponent<FkMatCompo>();

    int32_t offset = 0;
    FkVertexDesc desc;
    if (FK_OK != vboCompo->getValueLoc(FkVboCompo::kValueLoc::VERTEX, offset, desc)) {
        FkLogW(FK_DEF_TAG, "Get vertex count error.");
    }

    glViewport(0, 0, size.getWidth(), size.getHeight());
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    _drawBackground(p);
    FK_GL_CHECK(programCompo->program->bind());
    vboCompo->bind();

    auto backTexCompo = std::make_shared<FkTexCompo>(this->tex);
    programCompo->program->addValue(backTexCompo);
    programCompo->program->addValue(matCompo);
    programCompo->program->addValue(vboCompo);
    FK_GL_CHECK(glDrawArrays(GL_TRIANGLE_STRIP, 0, desc.countVertex));
    FK_GL_CHECK(programCompo->program->clear());

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

FkResult FkScreenQuark::_drawBackground(std::shared_ptr<FkProtocol> &p) {
    if (program) {
        FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkRenderProto, p);
        FK_GL_CHECK(program->bind());
        auto vboCompo = proto->materials->findComponent<FkVboCompo>();
        vboCompo->bind();
        int32_t offset = 0;
        FkVertexDesc desc;
        if (FK_OK != vboCompo->getValueLoc(FkVboCompo::kValueLoc::VERTEX, offset, desc)) {
            FkLogW(FK_DEF_TAG, "Get vertex count error.");
        }
        program->addValue(vboCompo);
        FK_GL_CHECK(glDrawArrays(GL_TRIANGLE_STRIP, 0, desc.countVertex));

        FK_GL_CHECK(program->clear());
        program->unbind();
        vboCompo->unbind();
        return FK_OK;
    }
    return FK_NPE;
}
