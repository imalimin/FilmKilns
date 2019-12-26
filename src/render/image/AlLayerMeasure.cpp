/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlLayerMeasure.h"
#include "AlVec4.h"
#include "AlMath.h"
#include "Logcat.h"

AlLayerMeasure::AlLayerMeasure() : Object() {

}

AlLayerMeasure::AlLayerMeasure(const AlLayerMeasure &o) : Object() {

}

AlLayerMeasure::~AlLayerMeasure() {

}

void AlLayerMeasure::updateOrthogonal(AlSize &src, AlSize &target) {
    _calculateRect(src, target, lRectF, cRectF);
    oMat.update(cRectF.left, cRectF.right, cRectF.bottom, cRectF.top, -1.0f, 1.0f);
}

void AlLayerMeasure::setScale(float scaleX, float scaleY) {
    tMat.setScale(scaleX, scaleY);
}

void AlLayerMeasure::setRotation(float rotation) {
    tMat.setRotation(rotation);
}

void AlLayerMeasure::setTranslate(float x, float y) {
    setTranslate(x, y, 0.0f);
}

void AlLayerMeasure::setTranslate(float x, float y, float alpha, float scaleX, float scaleY) {
    float nx = x * cRectF.getWidth() / 2.0f;
    float ny = y * cRectF.getHeight() / 2.0f;
    ///x = dx * cos(alpha) + dy * sin(alpha)
    ///y = -dx * sin(alpha) + dy * cos(alpha)
    tMat.setTranslate((nx * cos(alpha) + ny * sin(alpha)) * scaleX,
                      (-nx * sin(alpha) + ny * cos(alpha)) * scaleY);
}


void AlLayerMeasure::_calculateRect(AlSize &src, AlSize &target,
                                    AlRectF &srcRectF, AlRectF &targetRectF) {
    float aspectRatio = target.width > target.height ?
                        (float) target.width / (float) target.height :
                        (float) target.height / (float) target.width;

    /// 计算正交矩阵，小边为1，大边>1
    if (target.width > target.height) {
        targetRectF.left = -aspectRatio;
        targetRectF.right = -targetRectF.left;
        targetRectF.bottom = -1.0f;
        targetRectF.top = -targetRectF.bottom;
    } else {
        targetRectF.left = -1.0f;
        targetRectF.right = -targetRectF.left;
        targetRectF.bottom = -aspectRatio;
        targetRectF.top = -targetRectF.bottom;
    }
    /// 根据Canvas大小计算纹理顶点
    /// 保证图片总是完整填充到Canvas
    /// 并保证至Layer和Canvas至少有一边相等
    /// 此时layer model的scale=1为默认状态
    if (src.width / (float) src.height > target.width / (float) target.height) {
        srcRectF.left = targetRectF.left;
        srcRectF.right = -srcRectF.left;
        srcRectF.bottom = srcRectF.left * src.height / (float) src.width;
        srcRectF.top = -srcRectF.bottom;
    } else {
        srcRectF.bottom = targetRectF.bottom;
        srcRectF.top = -srcRectF.bottom;
        srcRectF.left = srcRectF.bottom * src.width / (float) src.height;
        srcRectF.right = -srcRectF.left;
    }
}

void AlLayerMeasure::measureTransLORectF(AlVec2 &leftTop, AlVec2 &leftBottom,
                                         AlVec2 &rightBottom, AlVec2 &rightTop) {
    AlMatrix mat = tMat * oMat;
    AlVec4 lt(lRectF.left, lRectF.top);
    AlVec4 lb(lRectF.left, lRectF.bottom);
    AlVec4 rb(lRectF.right, lRectF.bottom);
    AlVec4 rt(lRectF.right, lRectF.top);
    leftTop.set((lt * mat).xy());
    leftBottom.set((lb * mat).xy());
    rightBottom.set((rb * mat).xy());
    rightTop.set((rt * mat).xy());
}

HwResult AlLayerMeasure::measure(AlImageLayerDrawModel &drawModel) {
    drawModel.mat = tMat * oMat;
    drawModel.vertexRectF = lRectF;
    return Hw::SUCCESS;
}

AlRectF AlLayerMeasure::getSrcPosRectF() {
    return lRectF;
}

AlRectF AlLayerMeasure::getTargetPosRectF() {
    return cRectF;
}
