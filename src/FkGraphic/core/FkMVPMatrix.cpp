/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkMVPMatrix.h"

FkMVPMatrix::FkMVPMatrix(kProjType type)
        : FkMatrix(), type(type), proj(glm::mat4(1.0f)), view(glm::mat4(1.0f)),
          model(glm::mat4(1.0f)) {
    FK_MARK_SUPER
}

FkMVPMatrix::FkMVPMatrix(const FkMVPMatrix &o) : FkMatrix(o), type(o.type), proj(o.proj),
                                                 view(o.view), model(o.model) {
    FK_MARK_SUPER
}

FkMVPMatrix::~FkMVPMatrix() {

}

void FkMVPMatrix::setSize(int32_t width, int32_t height) {
    proj = glm::ortho(-width / 2.0f, width / 2.0f,
                      -height / 2.0f, height / 2.0f,
                      0.1f, 100.0f);
}
