/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKMVPMATRIX_H
#define FK_GRAPHIC_FKMVPMATRIX_H

#include "FkMatrix.h"
#include "FkFloatVec3.h"
#include "FkRational.h"

FK_SUPER_CLASS(FkMVPMatrix, FkMatrix) {
FK_DEF_CLASS_TYPE_FUNC(FkMVPMatrix)

public:
    FK_ENUM kProjType : uint32_t {
        ORTHO = 0,
        PERSPECTIVE,
    };
public:
    FkMVPMatrix(kProjType type);

    FkMVPMatrix(const FkMVPMatrix &o);

    virtual ~FkMVPMatrix();

    void setViewSize(int32_t width, int32_t height);

    void lookAt(FkFloatVec3 cameraPosition, FkFloatVec3 shootVec, FkFloatVec3 headVec);

    void setScale(FkFloatVec3 value);

    void setRotation(FkRational _radian);

    void setTranslate(FkFloatVec3 value);

    void calc();

private:
    kProjType type;
    glm::mat4 proj;
    glm::mat4 view;
    FkRational radian;
    FkFloatVec3 scale;
    FkFloatVec3 translate;
};

#endif //FK_GRAPHIC_FKMVPMATRIX_H