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
#include "AlLayerMeasurer.h"

al_class_ex(AlCropOperateModel, AlAbsOperateModel) {
public:
    AlCropOperateModel();

    AlCropOperateModel(const AlCropOperateModel &o);

    virtual ~AlCropOperateModel();

    virtual HwResult measure(AlImgLayerDescription &layer,
                             AlImageLayerDrawModel *description) override;

    void setRect(float left, float top, float right, float bottom);

    virtual HwResult fromElement(AlElement *element) override;

    virtual HwResult toElement(AlElement **element) override;

    AlRectF getRect();

    AlVec2 getScale();

    AlRational getRotation();

    AlVec2 getPosition();

    AlSize getCropSize();

    AlQuad getQuad();

    bool getInvalidate();

    /// Do not call this func. Just for restore.
    /// \param scale
    void setScale(AlVec2 scale);

    /// Do not call this func. Just for restore.
    /// \param rotation
    void setRotation(AlRational rotation);

    /// Do not call this func. Just for restore.
    /// \param position
    void setPosition(AlVec2 position);

    /// Do not call this func. Just for restore.
    /// \param size
    void setCropSize(AlSize size);

    /// Do not call this func. Just for restore.
    /// \param quad
    void setQuad(AlQuad quad);

    /// Do not call this func. Just for restore.
    /// \param invalidate
    void setInvalidate(bool invalidate);

private:
    bool invalidate = false;
    /// 裁剪框区域，相对于画布坐标系
    AlRectF rectF;
    AlVec2 scale;
    AlRational rotation;
    AlVec2 position;

    AlSize cropSize;
    AlQuad quad;
    AlLayerMeasurer aMeasurer;
};


#endif //HWVC_ANDROID_ALCROPOPERATEMODEL_H
