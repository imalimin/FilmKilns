/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
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
    }
    if (GL_NONE != id) {
        glDeleteTextures(1, &id);
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