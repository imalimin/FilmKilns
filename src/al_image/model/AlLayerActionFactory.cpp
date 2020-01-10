/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlLayerActionFactory.h"
#include "AlMCropAction.h"
#include "AlMAlignCropAction.h"
#include "AlMPaintAction.h"

AlAbsMAction *AlLayerActionFactory::crop(float left, float top, float right, float bottom) {
    auto *opt = new AlMCropAction();
    opt->setRect(left, top, right, bottom);
    return opt;
}

AlAbsMAction *AlLayerActionFactory::alignCrop(AlRational &r) {
    auto *opt = new AlMAlignCropAction();
    opt->setRotation(r);
    return opt;
}

AlAbsMAction *AlLayerActionFactory::paint(const float paintSize, const AlColor &color) {
    auto *action = new AlMPaintAction();
    action->setPaintSize(paintSize);
    action->setColor(color);
    return action;
}
