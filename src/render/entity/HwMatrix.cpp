/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwMatrix.h"
#include "Logcat.h"

HwMatrix *HwMatrix::fromArray(float *array) {
    HwMatrix *val = new HwMatrix();
    memcpy(val->matrix[0], array, 16 * sizeof(float));
    return val;
}

HwMatrix::HwMatrix() : Object() {

}

HwMatrix::HwMatrix(const HwMatrix &value) : Object() {
    memcpy(this->matrix[0], value.matrix[0], 16 * sizeof(float));
}

HwMatrix::~HwMatrix() {

}

HwMatrix &HwMatrix::operator=(const HwMatrix &value) {
    memcpy(this->matrix[0], value.matrix[0], 16 * sizeof(float));
    return *this;
}

float *HwMatrix::data() {
    return matrix[0];
}

void HwMatrix::scale(float sx, float sy, float sz) {
    matrix[0][0] = sx;
    matrix[1][1] = sy;
    matrix[2][2] = sz;
}

void HwMatrix::translate(float dx, float dy, float dz) {

}

void HwMatrix::rotate(float degrees) {

}

void HwMatrix::multiplyBy(HwMatrix *val) {
    HwMatrix tmp = *this;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            matrix[i][j] = multiply(&tmp, i, val, j);
        }
    }
}

float HwMatrix::multiply(HwMatrix *val1, int i, HwMatrix *val2, int j) {
    return val1->matrix[i][0] * val2->matrix[0][j]
           + val1->matrix[i][1] * val2->matrix[1][j]
           + val1->matrix[i][2] * val2->matrix[2][j]
           + val1->matrix[i][3] * val2->matrix[3][j];
}

void HwMatrix::reset() {
    float m[4][4] = {1.0f, 0.0f, 0.0f, 0.0f,
                     0.0f, 1.0f, 0.0f, 0.0f,
                     0.0f, 0.0f, 1.0f, 0.0f,
                     0.0f, 0.0f, 0.0f, 1.0f};
    memcpy(this->matrix[0], m[0], 16 * sizeof(float));
}