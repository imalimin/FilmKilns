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
#include "FkTexCompo.h"

FK_IMPL_CLASS_TYPE(FkTexDeviceQuark, FkQuark)

FkTexDeviceQuark::FkTexDeviceQuark() : FkQuark() {

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
    return FkQuark::onStop();
}

FkResult FkTexDeviceQuark::_onRender(std::shared_ptr<FkProtocol> p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkRenderProto, p);
    if (!FK_INSTANCE_OF(proto->device, FkTexDeviceEntity)) {
        return FK_SKIP;
    }
    FK_CAST_NULLABLE_PTR_RETURN_INT(device, FkTexDeviceEntity, proto->device);
    FK_CAST_NULLABLE_PTR_RETURN_INT(material, FkTexEntity, proto->materials);
    auto srcTexArray = material->texArray();
    auto fboCompo = material->fbo();
    auto dstTexArray = device->texArray();
    if (dstTexArray->empty()) {
        FkLogW(FK_DEF_TAG, "Not support for multi-texture frame buffer object.");
        return FK_SKIP;
    }
    auto size = device->size();

    auto programCompo = FK_FIND_COMPO(proto->materials, FkRenderProgramCompo);
    auto vboCompo = FK_FIND_COMPO(proto->materials, FkVboCompo);
    auto matCompo = FK_FIND_COMPO(proto->materials, FkMatCompo);
    FkAssert(vboCompo != nullptr, FK_FAIL);

    int32_t offset = 0;
    FkVertexDesc desc;
    if (FK_OK != vboCompo->getValueLoc(FkVboCompo::kValueLoc::VERTEX, offset, desc)) {
        FkLogW(FK_DEF_TAG, "Get vertex count error.");
    }

    glEnable(GL_BLEND);
    /**
     * Texture must be premultiplied
     * sfactor: texture
     * dfactor: frame buffer
     * blendColor = srcColor * sfactor + dstColor * dfactor
     * example:
     * glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
     * > blendColor.r = srcColor.a * srcColor.r + (1-srcColor.a) * dstColor.r
     * > blendColor.g = srcColor.a * srcColor.g + (1-srcColor.a) * dstColor.g
     * > blendColor.b = srcColor.a * srcColor.b + (1-srcColor.a) * dstColor.b
     * > blendColor.a = srcColor.a * srcColor.a + (1-srcColor.a) * dstColor.a
     */
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);

#if defined(__FK_DEBUG__)
    auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    FkAssert(GL_FRAMEBUFFER_COMPLETE == status,);
#endif
    vboCompo->bind();
    FK_GL_CHECK(programCompo->program->bind());
    FkIntVec2 pos(0, 0);
    for (int y = 0; y < dstTexArray->blocks.y; ++y) {
        pos.x = 0;
        for (int x = 0; x < dstTexArray->blocks.x; ++x) {
            int index = y * dstTexArray->blocks.x + x;
            auto tex = (*dstTexArray)[index];
            glViewport(-pos.x, -pos.y, size.getWidth(), size.getHeight());
            fboCompo->fbo->bind();
            fboCompo->fbo->attach(tex);
            programCompo->program->addValue(srcTexArray);
            programCompo->program->addValue(matCompo);
            programCompo->program->addValue(vboCompo);
            FK_GL_CHECK(glDrawArrays(GL_TRIANGLE_STRIP, 0, desc.countVertex));

            fboCompo->fbo->detach(tex->desc.target);
            fboCompo->fbo->unbind();
            FK_GL_CHECK(programCompo->program->clear());
            if (dstTexArray->blocks.x == x + 1) {
                pos.y += tex->desc.size.getHeight();
            }
            pos.x += tex->desc.size.getWidth();
        }
    }
    programCompo->program->unbind();
    vboCompo->unbind();
    glDisable(GL_BLEND);
    return FK_OK;
}