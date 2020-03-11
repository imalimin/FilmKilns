/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlLayerMeasurer.h"
#include "AlVec4.h"
#include "AlMath.h"
#include "Logcat.h"

#define TAG "AlLayerMeasurer"

const int32_t AlLayerMeasurer::SCALE_TYPE_ORIGINAL = 0;
const int32_t AlLayerMeasurer::SCALE_TYPE_CENTER_INSIDE = 1;
const int32_t AlLayerMeasurer::SCALE_TYPE_CENTER_CROP = 2;

AlLayerMeasurer &AlLayerMeasurer::original() {
    static AlLayerMeasurer *m;
    if (m == nullptr) {
        m = new AlLayerMeasurer(SCALE_TYPE_ORIGINAL);
    }
    return *m;
}

AlLayerMeasurer &AlLayerMeasurer::centerInside() {
    static AlLayerMeasurer *m;
    if (m == nullptr) {
        m = new AlLayerMeasurer(SCALE_TYPE_CENTER_INSIDE);
    }
    return *m;
}

AlLayerMeasurer::AlLayerMeasurer(int32_t scaleType) : Object(), scaleType(scaleType) {
}

AlLayerMeasurer::AlLayerMeasurer(const AlLayerMeasurer &o)
        : Object(),
          scaleType(o.scaleType),
          lRectF(o.lRectF),
          cRectF(o.cRectF),
          oMat(o.oMat),
          tMat(o.tMat) {

}

AlLayerMeasurer::~AlLayerMeasurer() {

}

void AlLayerMeasurer::updateOrthogonal(AlSize &src, AlSize &target) {
    _calculateRect(src, target, lRectF, cRectF);
    oMat.update(cRectF.left, cRectF.right, cRectF.bottom, cRectF.top, -1.0f, 1.0f);
    tMat.reset();
    Logcat::i(TAG, "canvas coord %f, %f", cRectF.left, cRectF.top);
}

void AlLayerMeasurer::setScale(float scaleX, float scaleY) {
    tMat.setScale(scaleX, scaleY);
}

void AlLayerMeasurer::setRotation(float rotation) {
    tMat.setRotation(rotation);
}

void AlLayerMeasurer::setTranslate(float x, float y) {
    setTranslate(x, y, 0.0f);
}

void AlLayerMeasurer::setTranslate(float x, float y, float alpha, float scaleX, float scaleY) {
    float nx = x * cRectF.getWidth() / 2.0f;
    float ny = y * cRectF.getHeight() / 2.0f;
    ///x = dx * cos(alpha) + dy * sin(alpha)
    ///y = -dx * sin(alpha) + dy * cos(alpha)
    tMat.setTranslate((nx * cos(alpha) + ny * sin(alpha)) * scaleX,
                      (-nx * sin(alpha) + ny * cos(alpha)) * scaleY);
}


void AlLayerMeasurer::_calculateRect(AlSize &src, AlSize &target,
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
    switch (scaleType) {
        case SCALE_TYPE_CENTER_INSIDE: {
            /// 根据Canvas大小计算纹理顶点
            /// 保证图片总是完整填充到Canvas
            /// 并保证至Layer和Canvas至少有一边相等
            /// 此时layer model的scale=1为默认状态
            if (src.width / (float) src.height > target.width / (float) target.height) {
                srcRectF.left = targetRectF.left;
                srcRectF.right = -srcRectF.left;
                srcRectF.bottom = targetRectF.left * src.height / (float) src.width;
                srcRectF.top = -srcRectF.bottom;
            } else {
                srcRectF.bottom = targetRectF.bottom;
                srcRectF.top = -srcRectF.bottom;
                srcRectF.left = targetRectF.bottom * src.width / (float) src.height;
                srcRectF.right = -srcRectF.left;
            }
            break;
        }
        case SCALE_TYPE_ORIGINAL:
        default: {
            /// 保证图层和画布pixel to pixel
            if (src.width / (float) src.height > target.width / (float) target.height) {
                srcRectF.left = targetRectF.left * src.width / target.width;
                srcRectF.right = -srcRectF.left;
                srcRectF.bottom = targetRectF.left * src.height / target.width;
                srcRectF.top = -srcRectF.bottom;
            } else {
                srcRectF.bottom = targetRectF.bottom * src.height / target.height;
                srcRectF.top = -srcRectF.bottom;
                srcRectF.left = targetRectF.bottom * src.width / target.height;
                srcRectF.right = -srcRectF.left;
            }
        }
    }
}

void AlLayerMeasurer::measureTransLORectF(AlVec2 &leftTop, AlVec2 &leftBottom,
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

HwResult AlLayerMeasurer::measurePoint(float &x, float &y) {
    AlMatrix mat = tMat * oMat;
    AlVec4 lt(x, y);
    AlVec2 vec = (lt * mat).xy();
    x = vec.x;
    y = vec.y;
}

HwResult AlLayerMeasurer::measure(AlImageLayerDrawModel &drawModel) {
    drawModel.mat = tMat * oMat;
    drawModel.vertexRectF = lRectF;
    return Hw::SUCCESS;
}

AlRectF AlLayerMeasurer::getSrcPosRectF() {
    return lRectF;
}

AlRectF AlLayerMeasurer::getTargetPosRectF() {
    return cRectF;
}
