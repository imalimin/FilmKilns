/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwMatrix.h"
#include <math.h>
#include "Logcat.h"

const float HwMatrix::PI = 3.141592653f;

HwMatrix *HwMatrix::fromArray(float *array) {
    HwMatrix *val = new HwMatrix();
    memcpy(val->matrix[0], array, 16 * sizeof(float));
    return val;
}

HwMatrix *HwMatrix::from(AlMatrix *src) {
    auto *dest = new HwMatrix();
    memcpy(dest->data(), src->data(), 16 * sizeof(float));
    return dest;
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
    matrix[0][3] = dx;
    matrix[1][3] = dy;
    matrix[2][3] = dz;
}

void HwMatrix::rotate(float dx, float dy, float dz) {
    HwMatrix m;
    // rotate x
    if (dx != 0.0f) {
        m.matrix[1][1] = static_cast<float>(cos(dx));
        m.matrix[1][2] = static_cast<float>(-sin(dx));
        m.matrix[2][1] = static_cast<float>(sin(dx));
        m.matrix[2][2] = static_cast<float>(cos(dx));
        m.matrix[3][1] = 1.0f;
        multiplyBy(&m);
    }
    // rotate y
    if (dy != 0.0f) {
        m.reset();
        m.matrix[0][0] = static_cast<float>(cos(dy));
        m.matrix[0][2] = static_cast<float>(sin(dy));
        m.matrix[2][0] = static_cast<float>(-sin(dy));
        m.matrix[2][2] = static_cast<float>(cos(dy));
        m.matrix[3][0] = 1.0f;
        multiplyBy(&m);
    }
    // rotate z
    if (dz != 0.0f) {
        m.reset();
        m.matrix[0][0] = static_cast<float>(cos(dz));
        m.matrix[0][1] = static_cast<float>(-sin(dz));
        m.matrix[1][0] = static_cast<float>(sin(dz));
        m.matrix[1][1] = static_cast<float>(cos(dz));
        m.matrix[3][1] = 1.0f;
        multiplyBy(&m);
    }
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
