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
#include "AlElement.h"
#include "AlAbsElemented.h"

#define TAG_OPT                             "opt"
#define VAL_TYPE                            "type"

al_class_ex(AlAbsOperateModel, AlAbsElemented) {
public:
    static const std::string TYPE_CROP;
    static const std::string TYPE_ALIGN_CROP;
public:
    AlAbsOperateModel(std::string type);

    AlAbsOperateModel(const AlAbsOperateModel &o);

    virtual ~AlAbsOperateModel();

    void setCanvasSize(AlSize size);

    AlSize getCanvasSize();

    std::string getType();

    virtual HwResult measure(AlImgLayerDescription &layer, AlImageLayerDrawModel *description) = 0;

protected:
    std::string type;
    AlSize canvasSize;
};


#endif //HWVC_ANDROID_ALABSOPERATEMODEL_H
