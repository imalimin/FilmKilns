/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALALIGNCROPOPERATEMODEL_H
#define HWVC_ANDROID_ALALIGNCROPOPERATEMODEL_H

#include "AlAbsOperateModel.h"

al_class_ex(AlAlignCropOperateModel, AlAbsOperateModel) {
public:
    AlAlignCropOperateModel();

    AlAlignCropOperateModel(const AlAlignCropOperateModel &o);

    virtual ~AlAlignCropOperateModel();

    virtual HwResult measure(AlImgLayerDescription &layer,
                             AlImageLayerDrawModel *description) override;

    void setRotation(AlRational &r);

private:
    AlRational rotation;
};


#endif //HWVC_ANDROID_ALALIGNCROPOPERATEMODEL_H
