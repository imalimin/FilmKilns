/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALORTHMATRIX_H
#define HWVC_ANDROID_ALORTHMATRIX_H

#include "AlMatrix.h"

al_class AlOrthMatrix al_extend AlMatrix {
public:
    AlOrthMatrix();

    AlOrthMatrix(const AlOrthMatrix &o);

    virtual ~AlOrthMatrix();

    void update(float left, float right, float bottom, float top, float near, float far);

};


#endif //HWVC_ANDROID_ALORTHMATRIX_H
