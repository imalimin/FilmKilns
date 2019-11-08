/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "../include/HwAbsFrameBuffer.h"

HwAbsFrameBuffer::HwAbsFrameBuffer(int w, int h) : Object() {
    size = new Size(w, h);
}

HwAbsFrameBuffer::~HwAbsFrameBuffer() {
    if (size) {
        delete size;
        size = nullptr;
    }
    if (size) {
        delete size;
        size = nullptr;
    }
    if (GL_NONE != fbo) {
        glDeleteFramebuffers(1, &fbo);
        fbo = GL_NONE;
    }
    if (GL_NONE != id) {
        glDeleteTextures(1, &id);
        id = GL_NONE;
    }
}

int HwAbsFrameBuffer::width() {
    if (size) {
        return size->width;
    }
    return 0;
}

int HwAbsFrameBuffer::height() {
    if (size) {
        return size->height;
    }
    return 0;
}

GLuint HwAbsFrameBuffer::getFrameTexture() {
    return id;
}

GLuint HwAbsFrameBuffer::getFrameBuffer() {
    return fbo;
}

bool HwAbsFrameBuffer::read(uint8_t *pixels) {
    if (!pixels) {
        return false;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glReadPixels(0, 0, width(), height(), fmt, GL_UNSIGNED_BYTE, pixels);
    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
    return true;
}