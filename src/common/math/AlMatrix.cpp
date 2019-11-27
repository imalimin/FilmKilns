//
// Created by mingyi.li on 2019-11-27.
//

#include "AlMatrix.h"
#include "Logcat.h"
#include <cmath>

#define TAG "AlMatrix"

const float AlMatrix::PI = 3.141592653f;
const int AlMatrix::SIZE = 16;

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
    _set(2, 3, 1.0f);
}

void AlMatrix::setRotation(float rotation) {
    AlMatrix mat;
    mat._set(0, 0, cosf(rotation) * _get(0, 0));
    mat._set(0, 1, -sinf(rotation) * _get(1, 1));
    mat._set(1, 0, sinf(rotation) * _get(0, 0));
    mat._set(1, 1, cosf(rotation) * _get(1, 1));
    memcpy(data(), mat.data(), SIZE * sizeof(float));
}

float *AlMatrix::data() {
    return matrix;
}

void AlMatrix::_set(int32_t x, int32_t y, float val) {
    matrix[x + y * 4] = val;
}

float AlMatrix::_get(int32_t x, int32_t y) {
    return matrix[x + y * 4];
}

void AlMatrix::setTranslate(float x, float y) {
    _set(0, 3, x);
    _set(1, 3, y);
}

void AlMatrix::dump() {
    Logcat::i(TAG, "1: {%f, %f, %f, %f}", _get(0, 0), _get(0, 1), _get(0, 2), _get(0, 3));
    Logcat::i(TAG, "2: {%f, %f, %f, %f}", _get(1, 0), _get(1, 1), _get(1, 2), _get(1, 3));
    Logcat::i(TAG, "3: {%f, %f, %f, %f}", _get(2, 0), _get(2, 1), _get(2, 2), _get(2, 3));
    Logcat::i(TAG, "4: {%f, %f, %f, %f}", _get(3, 0), _get(3, 1), _get(3, 2), _get(3, 3));
}
