/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALCROPOPERATEMODEL_H
#define HWVC_ANDROID_ALCROPOPERATEMODEL_H

#include "AlAbsOperateModel.h"
#include "AlRectF.h"
#include "AlRational.h"

al_class_ex(AlCropOperateModel, AlAbsOperateModel) {
public:
    AlCropOperateModel();

    AlCropOperateModel(const AlCropOperateModel &o);

    virtual ~AlCropOperateModel();

    virtual HwResult measure(AlAbsOperateModel::AlLayerDesc desc,
                             AlImageLayerDrawModel *description) override;

    void setRect(float left, float top, float right, float bottom);

    void setRotation(AlRational &r);

private:
    /// 计算正交矩阵
    /// \param src 图层大小
    /// \param dest 窗口大小
    void _calculateRect(AlSize &src, AlSize &dest, AlRectF &srcRectF, AlRectF &destRectF);

private:
    AlRectF rectF;
    AlRational rotation;
};


#endif //HWVC_ANDROID_ALCROPOPERATEMODEL_H
