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
#include "AlLayerMeasure.h"

al_class_ex(AlCropOperateModel, AlAbsOperateModel) {
public:
    AlCropOperateModel();

    AlCropOperateModel(const AlCropOperateModel &o);

    virtual ~AlCropOperateModel();

    virtual HwResult measure(AlAbsOperateModel::AlLayerDesc desc,
                             AlImageLayerDrawModel *description) override;

    void setRect(float left, float top, float right, float bottom);

    void setScale(float scaleX, float scaleY);

    void setRotation(AlRational &r);

    void setPosition(AlVec2 &position);

private:
    AlRectF rectF;
    AlRational rotation;
    AlVec2 position;
    AlVec2 scale;
    AlLayerMeasure aMeasure;
};


#endif //HWVC_ANDROID_ALCROPOPERATEMODEL_H
