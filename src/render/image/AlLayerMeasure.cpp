/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlLayerMeasure.h"
#include "AlVec4.h"

AlLayerMeasure::AlLayerMeasure() : Object() {

}

AlLayerMeasure::AlLayerMeasure(const AlLayerMeasure &o) : Object() {

}

AlLayerMeasure::~AlLayerMeasure() {

}

void AlLayerMeasure::update(AlSize &src, AlSize &dest) {
    _calculateRect(src, dest, lRectF, cRectF);
    oMat.update(cRectF.left, cRectF.right, cRectF.bottom, cRectF.top, -1.0f, 1.0f);
}

void AlLayerMeasure::setScale(float scaleX, float scaleY) {
    tMat.setScale(scaleX, scaleY);
}

void AlLayerMeasure::setRotation(float rotation) {
    tMat.setRotation(rotation);
}

void AlLayerMeasure::setTranslate(float x, float y) {
    tMat.setTranslate(x * cRectF.getWidth() / 2.0f, y * cRectF.getHeight() / 2.0f);
}


void AlLayerMeasure::_calculateRect(AlSize &src, AlSize &dest,
                                   AlRectF &srcRectF, AlRectF &destRectF) {
    float aspectRatio = dest.width > dest.height ?
                        (float) dest.width / (float) dest.height :
                        (float) dest.height / (float) dest.width;

    //计算正交矩阵
    if (dest.width > dest.height) {
        destRectF.left = -aspectRatio;
        destRectF.right = -destRectF.left;
        destRectF.bottom = -1.0f;
        destRectF.top = -destRectF.bottom;
    } else {
        destRectF.left = -1.0f;
        destRectF.right = -destRectF.left;
        destRectF.bottom = -aspectRatio;
        destRectF.top = -destRectF.bottom;
    }
    /// 根据Canvas大小计算纹理顶点
    /// 保证图片总是完整填充到Canvas
    /// 并保证至Layer和Canvas至少有一边相等
    /// 此时layer model的scale=1为默认状态
    if (src.width / (float) src.height > dest.width / (float) dest.height) {
        srcRectF.left = destRectF.left;
        srcRectF.right = -srcRectF.left;
        srcRectF.bottom = srcRectF.left * src.height / (float) src.width;
        srcRectF.top = -srcRectF.bottom;
    } else {
        srcRectF.bottom = destRectF.bottom;
        srcRectF.top = -srcRectF.bottom;
        srcRectF.left = srcRectF.bottom * src.width / (float) src.height;
        srcRectF.right = -srcRectF.left;
    }
}

AlMatrix AlLayerMeasure::getMatrix() {
    return oMat * tMat;
}

void AlLayerMeasure::getVertex(float *vertex) {
    vertex[0] = lRectF.left;
    vertex[1] = lRectF.bottom;
    vertex[2] = lRectF.right;
    vertex[3] = lRectF.bottom;
    vertex[4] = lRectF.left;
    vertex[5] = lRectF.top;
    vertex[6] = lRectF.right;
    vertex[7] = lRectF.top;
}

void AlLayerMeasure::getTransLORectF(AlVec2 &leftTop, AlVec2 &leftBottom,
                                    AlVec2 &rightBottom, AlVec2 &rightTop) {
    AlMatrix mat = getMatrix();
    AlVec4 lt(lRectF.left, lRectF.top);
    AlVec4 lb(lRectF.left, lRectF.bottom);
    AlVec4 rb(lRectF.right, lRectF.bottom);
    AlVec4 rt(lRectF.right, lRectF.top);
    leftTop.set(lt.multiply(mat).xy());
    leftBottom.set(lb.multiply(mat).xy());
    rightBottom.set(rb.multiply(mat).xy());
    rightTop.set(rt.multiply(mat).xy());
}
