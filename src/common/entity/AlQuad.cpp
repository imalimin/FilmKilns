/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlQuad.h"

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

void AlQuad::setLeftBottom(float x, float y) {
    _leftBottom.x = x;
    _leftBottom.y = y;
}

void AlQuad::setRightBottom(float x, float y) {
    _rightBottom.x = x;
    _rightBottom.y = y;
}

void AlQuad::setRightTop(float x, float y) {
    _rightTop.x = x;
    _rightTop.y = y;
}

AlPointF AlQuad::leftTop() {
    return _leftTop;
}

AlPointF AlQuad::leftBottom() {
    return _leftBottom;
}

AlPointF AlQuad::rightBottom() {
    return _rightBottom;
}

AlPointF AlQuad::rightTop() {
    return _rightTop;
}

bool AlQuad::contain(AlPointF pointF) {
    bool lt = pointF.x >= _leftTop.x && pointF.y <= _leftTop.y;
    bool lb = pointF.x >= _leftTop.x && pointF.y >= _leftBottom.y;
    bool rb = pointF.x <= _rightBottom.x && pointF.y >= _rightBottom.y;
    bool rt = pointF.x <= _rightTop.x && pointF.y <= _rightTop.y;
    return lt && lb && rb && rt;
}
