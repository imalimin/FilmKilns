/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkMatrix.h"
#include "ext.hpp"

FkMatrix::FkMatrix() : FkObject(), mat4(glm::mat4(1.0f)) {
    FK_MARK_SUPER
}

FkMatrix::FkMatrix(const FkMatrix &o) : FkObject(o), mat4(o.mat4) {
    FK_MARK_SUPER
}

FkMatrix::~FkMatrix() {
    glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
}

const void *FkMatrix::get() {
//    glm::mat4 proj = glm::perspective(glm::radians(0.0f), 1440.0f / 2526.0f, 0.1f, 100.0f);
//    glm::mat4 proj = glm::ortho(0.0f, 1440.0f, 0.0f, 2526.0f, 0.1f, 100.0f);
//    glm::mat4 view;
//    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
//    glm::mat4 model = glm::mat4(1.0f);
//    model  = glm::translate(model, glm::vec3(1.0f, 1.0f, 1.0f));
//    mat4 = proj * model;
    return glm::value_ptr(mat4);
}
