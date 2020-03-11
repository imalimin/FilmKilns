/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlMatrix.h"
#include "Logcat.h"
#include <cmath>

#define TAG "AlMatrix"

AlMatrix &AlMatrix::fromArray(float *array) {
    static AlMatrix *mat;
    if (mat == nullptr) {
        mat = new AlMatrix();
    }
    for (int i = 0; i < SIZE; ++i) {
        mat->matrix[i] = array[i];
    }
    return *mat;
}

AlMatrix::AlMatrix() : Object(), matrix(SIZE) {
    reset();
}

AlMatrix::AlMatrix(const AlMatrix &o) : Object(), matrix(o.matrix) {
}

AlMatrix::~AlMatrix() {

}

void AlMatrix::reset() {
    _set(0, 0, 1.f);
    _set(1, 1, 1.f);
    _set(2, 2, 1.f);
    _set(3, 3, 1.f);
}

void AlMatrix::setScale(float scaleX, float scaleY) {
    _set(0, 0, scaleX);
    _set(1, 1, scaleY);
    _set(3, 2, 1.0f);
}

void AlMatrix::setRotation(float rotation) {
    AlMatrix mat;
    mat._set(0, 0, cosf(rotation) * get(0, 0));
    mat._set(1, 0, -sinf(rotation) * get(1, 1));
    mat._set(0, 1, sinf(rotation) * get(0, 0));
    mat._set(1, 1, cosf(rotation) * get(1, 1));
    memcpy(data(), mat.data(), SIZE * sizeof(float));
}

float *AlMatrix::data() {
    return matrix.data();
}

void AlMatrix::_set(int32_t row, int32_t col, float val) {
    matrix[row * 4 + col] = val;
}

float AlMatrix::get(int32_t row, int32_t col) {
    return matrix[row * 4 + col];
}

void AlMatrix::setTranslate(float x, float y) {
    _set(3, 0, x);
    _set(3, 1, y);
}

AlMatrix &AlMatrix::T() {
    static AlMatrix *mat;
    if (mat == nullptr) {
        mat = new AlMatrix();
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            mat->_set(i, j, get(j, i));
        }
    }
    return *mat;
}

AlMatrix &AlMatrix::operator*(const AlMatrix &m) {
    static AlMatrix *mat;
    if (mat == nullptr) {
        mat = new AlMatrix();
    }
    AlMatrix *right = const_cast<AlMatrix *>(&m);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            mat->_set(i, j, get(i, 0) * right->get(0, j) +
                            get(i, 1) * right->get(1, j) +
                            get(i, 2) * right->get(2, j) +
                            get(i, 3) * right->get(3, j));
        }
    }
    return *mat;
}

AlVec4 &AlMatrix::operator*(const AlVec4 &v) {
    static AlVec4 *vec;
    if (vec == nullptr) {
        vec = new AlVec4();
    }
    for (int i = 0; i < 4; i++) {
        vec->set(i, v.x * get(i, 0) +
                    v.y * get(i, 1) +
                    v.z * get(i, 2) +
                    v.w * get(i, 3));
    }
    return *vec;
}

void AlMatrix::dump() {
    Logcat::i(TAG, "1: {%f, %f, %f, %f}", get(0, 0), get(0, 1), get(0, 2), get(0, 3));
    Logcat::i(TAG, "2: {%f, %f, %f, %f}", get(1, 0), get(1, 1), get(1, 2), get(1, 3));
    Logcat::i(TAG, "3: {%f, %f, %f, %f}", get(2, 0), get(2, 1), get(2, 2), get(2, 3));
    Logcat::i(TAG, "4: {%f, %f, %f, %f}", get(3, 0), get(3, 1), get(3, 2), get(3, 3));
}
