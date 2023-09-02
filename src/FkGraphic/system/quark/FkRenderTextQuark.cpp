/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2023-08-31 13:32:27
*/

#include "FkRenderTextQuark.h"
#include "FkRenderProto.h"
#include "FkTextCompo.h"
#include "FkTextureCharMap.h"
#include "FkTexEntity.h"
#include "FkRenderProgramCompo.h"
#include "FkCharMapCompo.h"
#include "FkGLDefinition.h"
#include "FkVboCompo.h"
#include "FkMatCompo.h"
#include "FkViewportMatCompo.h"

FK_IMPL_CLASS_TYPE(FkRenderTextQuark, FkQuark)

FkRenderTextQuark::FkRenderTextQuark() : FkQuark() {

}

FkRenderTextQuark::~FkRenderTextQuark() {

}
void FkRenderTextQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderProto, FkRenderTextQuark::_onRender);
}

FkResult FkRenderTextQuark::onCreate() {
    auto ret = FkQuark::onCreate();
    return ret;
}

FkResult FkRenderTextQuark::onDestroy() {
    return FkQuark::onDestroy();
}

FkResult FkRenderTextQuark::onStart() {
    auto ret = FkQuark::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkRenderTextQuark::onStop() {
    return FkQuark::onStop();
}

FkResult FkRenderTextQuark::_onRender(const std::shared_ptr<FkProtocol> &p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkRenderProto, p);
    if (!FK_INSTANCE_OF(proto->device, FkTexDeviceEntity)) {
        return FK_SKIP;
    }
    FK_CAST_NULLABLE_PTR_RETURN_INT(device, FkTexDeviceEntity, proto->device);
    FK_CAST_NULLABLE_PTR_RETURN_INT(materials, FkTexEntity, proto->materials);
    auto charMapCompo = FK_FIND_COMPO(materials, FkCharMapCompo);
    if (charMapCompo == nullptr) {
        return FK_OK;
    }
    auto fboCompo = materials->fbo();
    auto srcTexArray = std::make_shared<FkTexArrayCompo>(charMapCompo->charMap->getTexture());
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
    fboCompo->fbo->bind();
    programCompo->program->addValue(srcTexArray);
    programCompo->program->addValue(matCompo);
    programCompo->program->addValue(vboCompo);
    FkIntVec2 pos(0, 0);
    for (int y = 0; y < dstTexArray->blocks.y; ++y) {
        pos.x = 0;
        for (int x = 0; x < dstTexArray->blocks.x; ++x) {
            int index = y * dstTexArray->blocks.x + x;
            auto tex = (*dstTexArray)[index];
            auto compo = std::make_shared<FkViewportMatCompo>();
            compo->value = dstTexArray->calcViewportMatrix(index, pos);
            programCompo->program->addValue(compo);

            glViewport(0, 0, tex->desc.size.getWidth(), tex->desc.size.getHeight());
            fboCompo->fbo->attach(tex);
            FK_GL_CHECK(glDrawArrays(GL_TRIANGLE_STRIP, 0, desc.countVertex));

            fboCompo->fbo->detach(tex->desc.target);
            if (dstTexArray->blocks.x == x + 1) {
                pos.y += tex->desc.size.getHeight();
            }
            pos.x += tex->desc.size.getWidth();
        }
    }
    FK_GL_CHECK(programCompo->program->clear());
    fboCompo->fbo->unbind();
    programCompo->program->unbind();
    vboCompo->unbind();
    glDisable(GL_BLEND);
    return FK_DONE;
}