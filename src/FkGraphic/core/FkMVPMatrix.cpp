/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkMVPMatrix.h"
#include "ext.hpp"

FK_IMPL_CLASS_TYPE(FkMVPMatrix, FkObject)

FkMVPMatrix::FkMVPMatrix(kProjType type)
        : FkMatrix(), type(type), proj(glm::mat4(1.0f)), view(glm::mat4(1.0f)),
          radian(0, 1), scale(1.0f, 1.0f, 1.0f),
          translate(0.0f, 0.0f, 00.0f) {

}

FkMVPMatrix::FkMVPMatrix(const FkMVPMatrix &o) : FkMatrix(o), type(o.type), proj(o.proj),
                                                 view(o.view), radian(o.radian),
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
    mat4 = proj * view * model;
}
