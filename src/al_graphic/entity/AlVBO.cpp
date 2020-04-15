/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlVBO.h"
#include <GLES2/gl2.h>

AlVBO *AlVBO::alloc() {
    AlVBO *vbo = new AlVBO();
    if (GL_NONE == vbo->o) {
        delete vbo;
        return nullptr;
    }
    return vbo;
}

AlVBO::AlVBO() : Object() {
    glGenBuffers(1, &o);
    glBindBuffer(GL_ARRAY_BUFFER, o);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
}

AlVBO::~AlVBO() {
    if (GL_NONE != o) {
        glDeleteBuffers(1, &o);
        o = GL_NONE;
    }
    allocated = false;
}

void AlVBO::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, o);
}

void AlVBO::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
}

void AlVBO::update(AlBuffer *buf) {
    if (nullptr == buf) {
        return;;
    }
//    if (allocated) {
//        glBufferSubData(GL_ARRAY_BUFFER, 0, buf->size(), buf->data());
//    } else {
        allocated = true;
        glBufferData(GL_ARRAY_BUFFER, buf->size(), buf->data(), GL_STATIC_DRAW);
//    }
}
