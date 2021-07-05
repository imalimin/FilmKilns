/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKMVPMATRIX_H
#define FK_GRAPHIC_FKMVPMATRIX_H

#include "FkMatrix.h"

FK_CLASS FkMVPMatrix FK_EXTEND FkMatrix {
public:
    FK_ENUM kProjType : uint32_t {
        ORTHO = 0,
        PERSPECTIVE,
    };
public:
    FkMVPMatrix(kProjType type);

    FkMVPMatrix(const FkMVPMatrix &o);

    virtual ~FkMVPMatrix();

    void setSize(int32_t width, int32_t height);

//    void lookAt();

private:
    kProjType type;
    glm::mat4 proj;
    glm::mat4 view;
    glm::mat4 model;
};

#endif //FK_GRAPHIC_FKMVPMATRIX_H