/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_HWMATRIX_H
#define HWVC_ANDROID_HWMATRIX_H

#include "Object.h"

/**
 * 4 x 4 matrix.
 */
class HwMatrix : public Object {
public:
    static HwMatrix *fromArray(float *array);

public:
    HwMatrix();

    HwMatrix(const HwMatrix &value);

    ~HwMatrix();

    HwMatrix &operator=(const HwMatrix &value);

    float *data();

    void scale(float sx, float sy, float sz);

    void translate(float dx, float dy, float dz);

    void rotate(float degrees);

private:
    float matrix[16] = {1.0f, 0.0f, 0.0f, 0.0f,
                        0.0f, 1.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 1.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 1.0f};
};


#endif //HWVC_ANDROID_HWMATRIX_H
