/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicFrameObject.h"
#include "FkGLDefinition.h"
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>

FkGraphicFrameObject::FkGraphicFrameObject() : FkSource() {

}

FkGraphicFrameObject::~FkGraphicFrameObject() {

}

FkResult FkGraphicFrameObject::create() {
    glGenFramebuffers(1, &fbo);
    return GL_NO_ERROR == glGetError() ? FK_OK : FK_FAIL;
}

void FkGraphicFrameObject::destroy() {
#ifdef __FK_DEBUG__
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

FkResult FkGraphicFrameObject::attach(std::shared_ptr<FkGraphicTexture> o) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, o->desc.target, o->tex, 0);
    return GL_NO_ERROR == glGetError() ? FK_OK : FK_FAIL;
}

FkResult FkGraphicFrameObject::detach(uint32_t target) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, target, GL_NONE, 0);
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
