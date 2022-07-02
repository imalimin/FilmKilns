/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_BASE_FKMATRIX_H
#define FK_BASE_FKMATRIX_H

#include "FkObject.h"
#include "FkDoubleVec2.h"
#include "FkFloatVec3.h"
#include "FkRational.h"

FK_SUPER_CLASS(FkMatrix, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkMatrix)

public:
    FkMatrix();

    FkMatrix(const FkMatrix &o);

    virtual ~FkMatrix();

    virtual const void *get() = 0;

    virtual void setViewSize(int32_t width, int32_t height) = 0;

    virtual void lookAt(FkFloatVec3 cameraPosition, FkFloatVec3 shootVec, FkFloatVec3 headVec) = 0;

    virtual void setScale(FkFloatVec3 value) = 0;

    virtual void setRotation(FkRational _radian) = 0;

    virtual void setTranslate(FkFloatVec3 value) = 0;

    virtual void calc() = 0;

    virtual FkDoubleVec2 calcVec2(FkDoubleVec2 &vec2) = 0;

    virtual std::string toString() override;
};

#endif //FK_BASE_FKMATRIX_H