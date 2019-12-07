/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlOperateFactory.h"

AlCropOperateModel *AlOperateFactory::crop(float left, float top, float right, float bottom) {
    auto *opt = new AlCropOperateModel();
    opt->setRect(left, top, right, bottom);
    return opt;
}
