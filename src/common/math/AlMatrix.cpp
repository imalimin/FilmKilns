//
// Created by mingyi.li on 2019-11-27.
//

#include "AlMatrix.h"
#include "Logcat.h"
#include <cmath>

#define TAG "AlMatrix"

const float AlMatrix::PI = 3.141592653f;
const int AlMatrix::SIZE = 16;

AlMatrix &AlMatrix::fromArray(float *array) {
    static AlMatrix *mat;
    if (mat == nullptr) {
        mat = new AlMatrix();
    }
    memcpy(mat->matrix, array, SIZE * sizeof(float));
    return *mat;
}

AlMatrix::AlMatrix() : Object() {

}

AlMatrix::AlMatrix(const AlMatrix &o) : Object() {
    memcpy(this->matrix, o.matrix, SIZE * sizeof(float));
}

AlMatrix::~AlMatrix() {

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
    return matrix;
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

AlMatrix &AlMatrix::operator*(AlMatrix m) {
    static AlMatrix *mat;
    if (mat == nullptr) {
        mat = new AlMatrix();
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            mat->_set(i, j, get(i, 0) * m.get(0, j) +
                            get(i, 1) * m.get(1, j) +
                            get(i, 2) * m.get(2, j) +
                            get(i, 3) * m.get(3, j));
        }
    }
    return *mat;
}

void AlMatrix::dump() {
    Logcat::i(TAG, "1: {%f, %f, %f, %f}", get(0, 0), get(0, 1), get(0, 2), get(0, 3));
    Logcat::i(TAG, "2: {%f, %f, %f, %f}", get(1, 0), get(1, 1), get(1, 2), get(1, 3));
    Logcat::i(TAG, "3: {%f, %f, %f, %f}", get(2, 0), get(2, 1), get(2, 2), get(2, 3));
    Logcat::i(TAG, "4: {%f, %f, %f, %f}", get(3, 0), get(3, 1), get(3, 2), get(3, 3));
}
