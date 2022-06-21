/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicFrameObject.h"
#include "FkGLDefinition.h"
#include <EGL/egl.h>

FK_IMPL_CLASS_TYPE(FkGraphicFrameObject, FkSource)
FK_IMPL_CLASS_TYPE(FkGraphicFBOAllocator, FkSourceAllocator)

FkGraphicFrameObject::FkGraphicFrameObject() : FkSource() {

}

FkGraphicFrameObject::~FkGraphicFrameObject() {

}

FkResult FkGraphicFrameObject::create() {
    glGenFramebuffers(1, &fbo);
    return GL_NO_ERROR == glGetError() ? FK_OK : FK_FAIL;
}

void FkGraphicFrameObject::destroy() {
#if defined(__FK_DEBUG__)
    FkAssert(EGL_NO_CONTEXT != eglGetCurrentContext(), );
#endif
    glDeleteFramebuffers(1, &fbo);
}

size_t FkGraphicFrameObject::size() {
    return 1;
}

void FkGraphicFrameObject::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void FkGraphicFrameObject::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

FkResult FkGraphicFrameObject::attach(std::shared_ptr<FkGraphicTexture> &tex) {
    std::vector<std::shared_ptr<FkGraphicTexture>> texArray = {tex};
    return attach(texArray);
}

FkResult FkGraphicFrameObject::attach(std::vector<std::shared_ptr<FkGraphicTexture>> &texArray) {
    if (!attachments.empty()) {
        FkLogI(FK_DEF_TAG, "You must detach this fbo before attach.");
        return FK_INVALID_STATE;
    }
    for (int i = 0; i < texArray.size(); ++i) {
        auto attachment = GL_COLOR_ATTACHMENT0 + i;
        attachments.emplace_back(attachment);
        auto tex = texArray[i];
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, tex->desc.target, tex->tex, 0);
    }
    glDrawBuffers(attachments.size(), attachments.data());
    return GL_NO_ERROR == glGetError() ? FK_OK : FK_FAIL;
}

FkResult FkGraphicFrameObject::detach(uint32_t target) {
    for (int i = 0; i < attachments.size(); ++i) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachments[i], target, GL_NONE, 0);
    }
    attachments.clear();
    return GL_NO_ERROR == glGetError() ? FK_OK : FK_FAIL;
}

FkGraphicFBOAllocator::FkGraphicFBOAllocator(int capacity)
        : FkSourceAllocator<FkGraphicFrameObject, int32_t>(capacity) {

}

FkGraphicFBOAllocator::~FkGraphicFBOAllocator() {

}

FkGraphicFrameObject *FkGraphicFBOAllocator::delegateAlloc(int32_t &desc) {
    auto o = new FkGraphicFrameObject();
    o->create();
    return o;
}

bool FkGraphicFBOAllocator::delegateEquals(int32_t &desc, FkGraphicFrameObject *fbo) {
    return true;
}
