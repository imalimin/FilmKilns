/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALLAYERMEASURER_H
#define HWVC_ANDROID_ALLAYERMEASURER_H

#include "Object.h"
#include "AlOrthMatrix.h"
#include "AlRectF.h"
#include "AlSize.h"
#include "AlVec2.h"
#include "HwResult.h"
#include "AlImageLayerDrawModel.h"

/// 用于计算图层和画布进行正交投影，转换坐标系，归一化区域
/// 计算后可以获得以下信息
/// 1.画布的正交投影归一化坐标
/// 2.图层的正交投影归一化坐标
/// 3.GL变换矩阵
/// 4.GL顶点
/// 5.变换后新图像的四个位置坐标
al_class(AlLayerMeasurer) {
public:
    AlLayerMeasurer();

    AlLayerMeasurer(const AlLayerMeasurer &o);

    ~AlLayerMeasurer();

    /// 对target size进行归一化，并把src size正交变换到target坐标范围
    /// 比如对图层和画布进行正交投影变化
    /// \param src 图层大小
    /// \param dest 画布大小
    void updateOrthogonal(AlSize &src, AlSize &target);

    void setScale(float scaleX, float scaleY);

    void setRotation(float rotation);

    /// 设置矩阵偏移量
    /// \param x      x轴偏移量
    /// \param y      y轴偏移量
    void setTranslate(float x, float y);

    /// 设置矩阵偏移量
    /// 由于在做角度转换之前，需要先乘以cRectF，此时需要使用该方法
    /// \param x      x轴偏移量
    /// \param y      y轴偏移量
    /// \param alpha  坐标轴旋转角度，负数为CW
    /// \param scaleX x轴偏移量放大系数，目前只用于正负转换
    /// \param scaleY y轴偏移量放大系数，目前只用于正负转换
    ///
    void setTranslate(float x, float y, float alpha, float scaleX = 1.0f, float scaleY = 1.0f);

    /// 获取图层变换后的归一化坐标
    /// 与Shader中的uTextureMatrix * aPosition一致
    /// \param leftTop
    /// \param leftBottom
    /// \param rightBottom
    /// \param rightTop
    void measureTransLORectF(AlVec2 &leftTop, AlVec2 &leftBottom,
                         AlVec2 &rightBottom, AlVec2 &rightTop);

    /// 测量图层运用正交投影，缩放旋转位移后的一些信息，比如最终的变换矩阵，用于绘制图层
    /// \param drawModel 图层绘制信息
    /// \return
    HwResult measure(AlImageLayerDrawModel &drawModel);

    /// 获取经过正交变换后，src size的四个顶点坐标
    /// 比如图层在画布坐标系中四个顶点的值
    AlRectF getSrcPosRectF();

    /// 获取经过正交变换后，坐标系的四个顶点
    /// 比如画布的四个顶点范围
    AlRectF getTargetPosRectF();

private:
    /// 计算正交矩阵
    /// \param src 图层大小
    /// \param dest 窗口大小
    void _calculateRect(AlSize &src, AlSize &target, AlRectF &srcRectF, AlRectF &targetRectF);

private:
    ///src,正交投影变换后图层归一化后的位置坐标，没有进行缩放旋转位移
    AlRectF lRectF;
    ///target,正交投影变换后画布归一化后的可视位置坐标
    AlRectF cRectF;
    ///用于变换操作的矩阵
    ///Orthogonal matrix
    AlOrthMatrix oMat;
    ///Translate matrix
    AlMatrix tMat;
};


#endif //HWVC_ANDROID_ALLAYERMEASURER_H
