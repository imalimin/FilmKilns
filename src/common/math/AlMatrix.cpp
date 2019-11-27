//
// Created by mingyi.li on 2019-11-27.
//

#include "AlMatrix.h"

const float AlMatrix::PI = 3.141592653f;
const int AlMatrix::SIZE = 16;

AlMatrix::AlMatrix() : Object() {

}

AlMatrix::AlMatrix(const AlMatrix &o) : Object() {
    memcpy(this->matrix[0], o.matrix[0], SIZE * sizeof(float));
}

AlMatrix::~AlMatrix() {

}

void AlMatrix::setScale(float scaleX, float scaleY) {
    matrix[0][0] = scaleX;
    matrix[1][1] = scaleY;
    matrix[2][2] = 1.0f;
}

void AlMatrix::setRotation(float rotation) {

}

float *AlMatrix::data() {
    return matrix[0];
}
