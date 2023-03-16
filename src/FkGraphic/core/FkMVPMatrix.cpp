/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkMVPMatrix.h"
#include "ext/matrix_clip_space.hpp"
#include "ext.hpp"

FK_IMPL_CLASS_TYPE(FkMVPMatrix, FkObject)

FkMVPMatrix::FkMVPMatrix(kProjType type)
        : FkMatrix(), type(type), mat4(glm::mat4(1.0f)), view(glm::mat4(1.0f)), proj(glm::mat4(1.0f)),
          radian(0, 1), scale(1.0f, 1.0f, 1.0f),
          translate(0.0f, 0.0f, 0.0f) {

}

FkMVPMatrix::FkMVPMatrix(float *data)
        : FkMatrix(), mat4(glm::make_mat4(data)), view(glm::mat4(1.0f)), proj(glm::mat4(1.0f)),
          radian(0, 1), scale(1.0f, 1.0f, 1.0f),
          translate(0.0f, 0.0f, 0.0f) {
}

FkMVPMatrix::FkMVPMatrix(const FkMVPMatrix &o) : FkMatrix(o), type(o.type), mat4(o.proj),
                                                 proj(o.proj), view(o.view), radian(o.radian),
                                                 scale(o.scale), translate(o.translate) {

}

FkMVPMatrix::~FkMVPMatrix() {

}

void FkMVPMatrix::setViewSize(int32_t width, int32_t height) {
    proj = glm::ortho(-width / 2.0f, width / 2.0f,
                      -height / 2.0f, height / 2.0f,
                      0.1f, 100.0f);
}

void FkMVPMatrix::lookAt(FkFloatVec3 cameraPosition, FkFloatVec3 shootVec, FkFloatVec3 headVec) {
    view = glm::lookAt(
            glm::vec3(cameraPosition.x, cameraPosition.y, cameraPosition.z), // Camera is at (0,0,3), in World Space
            glm::vec3(shootVec.x, shootVec.y, shootVec.z), // and looks at the origin
            glm::vec3(headVec.x, headVec.y, headVec.z)  // Head is up (set to 0,-1,0 to look upside-down)
    );
}

void FkMVPMatrix::setScale(FkFloatVec3 value) {
    this->scale = value;
}

void FkMVPMatrix::setRotation(FkRational _radian) {
    this->radian = _radian;
}

void FkMVPMatrix::setTranslate(FkFloatVec3 value) {
    this->translate = value;
}

void FkMVPMatrix::calc() {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, -radian.num *1.0f / radian.den, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(scale.x, scale.y, scale.z));
    model = glm::translate(model, glm::vec3(translate.x, translate.y, translate.z));
    if (proj == glm::mat4(1.0f) && view == glm::mat4(1.0f)) {
        mat4 = model;
    } else {
        mat4 = proj * view * model;
    }
}

const void *FkMVPMatrix::get() {
//    glm::mat4 proj = glm::perspective(glm::radians(0.0f), 1440.0f / 2526.0f, 0.1f, 100.0f);
//    glm::mat4 proj = glm::ortho(0.0f, 1440.0f, 0.0f, 2526.0f, 0.1f, 100.0f);
//    glm::mat4 view;
//    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
//    glm::mat4 model = glm::mat4(1.0f);
//    model  = glm::translate(model, glm::vec3(1.0f, 1.0f, 1.0f));
//    mat4 = proj * model;
    return glm::value_ptr(mat4);
}

FkDoubleVec2 FkMVPMatrix::calcVec2(FkDoubleVec2 &vec2) {
    calc();
    glm::vec4 vec(vec2.x, vec2.y, 0, 1.0f);
    vec = vec * mat4;
    return FkDoubleVec2(vec.x, vec.y);
}
