/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwAbsFBObject.h"
#include <GLES2/gl2.h>

HwAbsFBObject::HwAbsFBObject() : Object() {
    glGenFramebuffers(1, &fbo);
}

HwAbsFBObject::~HwAbsFBObject() {
    this->tex = nullptr;
}

void HwAbsFBObject::bindTex(HwTexture *tex) {
    if (this->tex == tex) {
        return;
    }
    this->tex = tex;
    if (this->tex) {
        bind();
        this->tex->bind();
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               tex->target(), tex->texId(), 0);
        this->tex->unbind();
        unbind();
    }
}

void HwAbsFBObject::unbindTex() {
    if (this->tex) {
        bind();
        this->tex->bind();
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               tex->target(), GL_NONE, 0);
        this->tex->unbind();
        unbind();
        this->tex = nullptr;
    }
}

void HwAbsFBObject::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void HwAbsFBObject::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}