/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlQuad.h"
#include "AlLogcat.h"

#define TAG "AlQuad"

AlQuad::AlQuad() : Object() {
    setLeftTop(0.0f, 0.0f);
    setLeftBottom(0.0f, 0.0f);
    setRightBottom(0.0f, 0.0f);
    setRightTop(0.0f, 0.0f);
}

AlQuad::AlQuad(const AlQuad &o) : Object(),
                                  _leftTop(o._leftTop),
                                  _leftBottom(o._leftBottom),
                                  _rightBottom(o._rightBottom),
                                  _rightTop(o._rightTop) {

}

AlQuad::~AlQuad() {
    setLeftTop(0.0f, 0.0f);
    setLeftBottom(0.0f, 0.0f);
    setRightBottom(0.0f, 0.0f);
    setRightTop(0.0f, 0.0f);
}

void AlQuad::setLeftTop(float x, float y) {
    _leftTop.x = x;
    _leftTop.y = y;
}

void AlQuad::setLeftTop(AlVec2 vec2) {
    setLeftTop(vec2.x, vec2.y);
}

void AlQuad::setLeftBottom(float x, float y) {
    _leftBottom.x = x;
    _leftBottom.y = y;
}

void AlQuad::setLeftBottom(AlVec2 vec2) {
    setLeftBottom(vec2.x, vec2.y);
}

void AlQuad::setRightBottom(float x, float y) {
    _rightBottom.x = x;
    _rightBottom.y = y;
}

void AlQuad::setRightBottom(AlVec2 vec2) {
    setRightBottom(vec2.x, vec2.y);
}

void AlQuad::setRightTop(float x, float y) {
    _rightTop.x = x;
    _rightTop.y = y;
}

void AlQuad::setRightTop(AlVec2 vec2) {
    setRightTop(vec2.x, vec2.y);
}

AlPointF &AlQuad::leftTop() {
    return _leftTop;
}

AlPointF &AlQuad::leftBottom() {
    return _leftBottom;
}

AlPointF &AlQuad::rightBottom() {
    return _rightBottom;
}

AlPointF &AlQuad::rightTop() {
    return _rightTop;
}

/// 四边形内的点都在顺时针（逆时针）向量的同一边
/// 即夹角小于90o，向量积同向
/// \param pointF 要求解的点
/// \return 是否在四边形内部
bool AlQuad::inside(AlPointF pointF) {
    float a = (_leftTop.x - _rightTop.x) * (pointF.y - _rightTop.y)
              - (_leftTop.y - _rightTop.y) * (pointF.x - _rightTop.x);
    float b = (_rightTop.x - _rightBottom.x) * (pointF.y - _rightBottom.y)
              - (_rightTop.y - _rightBottom.y) * (pointF.x - _rightBottom.x);
    float c = (_rightBottom.x - _leftBottom.x) * (pointF.y - _leftBottom.y)
              - (_rightBottom.y - _leftBottom.y) * (pointF.x - _leftBottom.x);
    float d = (_leftBottom.x - _leftTop.x) * (pointF.y - _leftTop.y)
              - (_leftBottom.y - _leftTop.y) * (pointF.x - _leftTop.x);
    return (a > 0 && b > 0 && c > 0 && d > 0) || (a < 0 && b < 0 && c < 0 && d < 0);
}

AlQuad &AlQuad::mirrorVertical() {
    AlVec2::swap(&_leftTop, &_leftBottom);
    AlVec2::swap(&_rightTop, &_rightBottom);
    _leftTop.y = -_leftTop.y;
    _leftBottom.y = -_leftBottom.y;
    _rightBottom.y = -_rightBottom.y;
    _rightTop.y = -_rightTop.y;
    return *this;
}

AlQuad &AlQuad::mirrorHorizontal() {
    return *this;
}

bool AlQuad::isZero() {
    return _leftTop.isZero() && _leftBottom.isZero()
           && _rightBottom.isZero() && _rightTop.isZero();
}

void AlQuad::dump() {
    AlLogI(TAG, "top(%f,%f), (%f,%f)\nbot(%f,%f), (%f,%f)",
           _leftTop.x, _leftTop.y,
           _rightTop.x, _rightTop.y,
           _leftBottom.x, _leftBottom.y,
           _rightBottom.x, _rightBottom.y);
}
