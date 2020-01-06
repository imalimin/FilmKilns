/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlOperateFactory.h"
#include "AlMCropAction.h"
#include "AlMAlignCropAction.h"

AlAbsMAction *AlOperateFactory::crop(float left, float top, float right, float bottom) {
    auto *opt = new AlMCropAction();
    opt->setRect(left, top, right, bottom);
    return opt;
}

AlAbsMAction *AlOperateFactory::alignCrop(AlRational &r) {
    auto *opt = new AlMAlignCropAction();
    opt->setRotation(r);
    return opt;
}
