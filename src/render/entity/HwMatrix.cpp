/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwMatrix.h"

HwMatrix *HwMatrix::fromArray(float *array) {
    HwMatrix *val = new HwMatrix();
    memcpy(val->matrix, array, 16 * sizeof(float));
    return val;
}

HwMatrix::HwMatrix() : Object() {

}

HwMatrix::HwMatrix(const HwMatrix &value) : Object() {
    memcpy(this->matrix, value.matrix, 16 * sizeof(float));
}

HwMatrix::~HwMatrix() {

}

HwMatrix &HwMatrix::operator=(const HwMatrix &value) {
    memcpy(this->matrix, value.matrix, 16 * sizeof(float));
    return *this;
}

float *HwMatrix::data() {
    return matrix;
}

void HwMatrix::scale(float sx, float sy, float sz) {

}

void HwMatrix::translate(float dx, float dy, float dz) {

}

void HwMatrix::rotate(float degrees) {

}