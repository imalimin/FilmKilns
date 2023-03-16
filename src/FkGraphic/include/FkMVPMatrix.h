/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKMVPMATRIX_H
#define FK_GRAPHIC_FKMVPMATRIX_H

#include "FkMatrix.h"
#include "glm.hpp"

FK_SUPER_CLASS(FkMVPMatrix, FkMatrix) {
FK_DEF_CLASS_TYPE_FUNC(FkMVPMatrix)

public:
    FK_ENUM kProjType : uint32_t {
        ORTHO = 0,
        PERSPECTIVE,
    };
public:
    FkMVPMatrix(kProjType type);

    FkMVPMatrix(float *data);

    FkMVPMatrix(const FkMVPMatrix &o);

    virtual ~FkMVPMatrix();

    virtual void setViewSize(int32_t width, int32_t height) override;

    virtual void lookAt(FkFloatVec3 cameraPosition,
                        FkFloatVec3 shootVec,
                        FkFloatVec3 headVec) override;

    virtual void setScale(FkFloatVec3 value) override;

    virtual void setRotation(FkRational _radian) override;

    virtual void setTranslate(FkFloatVec3 value) override;

    virtual void calc() override;

    virtual FkDoubleVec2 calcVec2(FkDoubleVec2 &vec2) override;

    virtual const void *get() override;

private:
    glm::mat4 mat4;
    kProjType type;
    glm::mat4 proj;
    glm::mat4 view;
    FkRational radian;
    FkFloatVec3 scale;
    FkFloatVec3 translate;
};

#endif //FK_GRAPHIC_FKMVPMATRIX_H