/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALABSOPERATEMODEL_H
#define HWVC_ANDROID_ALABSOPERATEMODEL_H

#include "Object.h"
#include "HwResult.h"
#include "AlRational.h"
#include "AlImgLayerDescription.h"
#include "AlImageLayerDrawModel.h"

al_class(AlAbsOperateModel) {
public:
    AlAbsOperateModel();

    AlAbsOperateModel(const AlAbsOperateModel &o);

    virtual ~AlAbsOperateModel();

    void setCanvasSize(AlSize size);

    virtual HwResult measure(AlImgLayerDescription &layer, AlImageLayerDrawModel *description) = 0;

protected:
    AlSize canvasSize;
};


#endif //HWVC_ANDROID_ALABSOPERATEMODEL_H
