/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALORTHMATRIX_H
#define HWVC_ANDROID_ALORTHMATRIX_H

#include "AlMatrix.h"

al_class_ex(AlOrthMatrix, AlMatrix) {
public:
    AlOrthMatrix();

    AlOrthMatrix(const AlOrthMatrix &o);

    virtual ~AlOrthMatrix();

    void setScale(float scaleX, float scaleY) override;

    void setRotation(float rotation) override;

    void setTranslate(float x, float y) override;

    float *data() override;

    void update(float left, float right, float bottom, float top, float near, float far);

private:
    AlMatrix vMat;
    AlMatrix pMat;
    //变换矩阵
    AlMatrix tMat;
    //正交投影矩阵
    AlMatrix oMat;
};


#endif //HWVC_ANDROID_ALORTHMATRIX_H
