/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlOperateFactory.h"
#include "AlCropOperateModel.h"
#include "AlAlignCropOperateModel.h"

AlAbsOperateModel *AlOperateFactory::crop(float left, float top, float right, float bottom) {
    auto *opt = new AlCropOperateModel();
    opt->setRect(left, top, right, bottom);
    return opt;
}

AlAbsOperateModel *AlOperateFactory::alignCrop(AlRational &r) {
    auto *opt = new AlAlignCropOperateModel();
    opt->setRotation(r);
    return opt;
}
