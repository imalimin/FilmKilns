/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkMVPMatrix.h"
#include "ext.hpp"

FkMVPMatrix::FkMVPMatrix(kProjType type)
        : FkMatrix(), type(type), proj(glm::mat4(1.0f)), view(glm::mat4(1.0f)),
          rotateAngle(0.0f), scale(1.0f, 1.0f, 1.0f),
          translate(0.0f, 0.0f, 00.0f) {
    FK_MARK_SUPER
}

FkMVPMatrix::FkMVPMatrix(const FkMVPMatrix &o) : FkMatrix(o), type(o.type), proj(o.proj),
                                                 view(o.view), rotateAngle(o.rotateAngle),
                                                 scale(o.scale), translate(o.translate) {
    FK_MARK_SUPER
}

FkMVPMatrix::~FkMVPMatrix() {

}

void FkMVPMatrix::setViewSize(int32_t width, int32_t height) {
    proj = glm::ortho(-width / 2.0f, width / 2.0f,
                      -height / 2.0f, height / 2.0f,
                      0.1f, 100.0f);
    _calc();
}

void FkMVPMatrix::lookAt(FkFloatVec3 cameraPosition, FkFloatVec3 shootVec, FkFloatVec3 headVec) {
    view = glm::lookAt(
            glm::vec3(cameraPosition.x, cameraPosition.y, cameraPosition.z), // Camera is at (0,0,3), in World Space
            glm::vec3(shootVec.x, shootVec.y, shootVec.z), // and looks at the origin
            glm::vec3(headVec.x, headVec.y, headVec.z)  // Head is up (set to 0,-1,0 to look upside-down)
    );
    _calc();
}

void FkMVPMatrix::setScale(FkFloatVec3 value) {
    this->scale = value;
    _calc();
}

void FkMVPMatrix::setRotation(float angle) {
    this->rotateAngle = angle;
    _calc();
}

void FkMVPMatrix::setTranslate(FkFloatVec3 value) {
    this->translate = value;
    _calc();
}

void FkMVPMatrix::_calc() {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, rotateAngle, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(scale.x, scale.y, scale.z));
    model = glm::translate(model, glm::vec3(translate.x, translate.y, translate.z));
    mat4 = proj * view * model;
}
