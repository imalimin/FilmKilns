/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicFrameObject.h"
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>

FkGraphicFrameObject::FkGraphicFrameObject() : FkSource() {

}

FkGraphicFrameObject::~FkGraphicFrameObject() {

}

FkResult FkGraphicFrameObject::create() {
    glGenFramebuffers(1, &fbo);
    return GL_NO_ERROR != glGetError();
}

void FkGraphicFrameObject::destroy() {
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
    bind();
    o->bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, o->desc.target, o->tex, 0);
    o->unbind();
    unbind();
    return GL_NO_ERROR != glGetError();
}

FkResult FkGraphicFrameObject::dettach(uint32_t target) {
    bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, target, GL_NONE, 0);
    unbind();
    return GL_NO_ERROR != glGetError();
}

FkGraphicFBOAllocator::FkGraphicFBOAllocator()
        : FkSourceAllocator<FkGraphicFrameObject, int32_t>() {

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
