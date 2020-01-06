/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlCoordsTranslator.h"

void AlCoordsTranslator::translate(AlSize src, AlSize target, float &x, float &y) {
    float winRatio = src.width / (float) src.height;
    float cRatio = target.width / (float) target.height;
    if (winRatio > cRatio) {
//        float scale = cRatio / winRatio;
//        x = x / scale;
        x = (x * src.width * target.height) / (float) (target.width * src.height);
    } else {
//        float scale = winRatio / cRatio;
//        y = y / scale;
        y = (y * target.width * src.height) / (float) (src.width * target.height);
    }

}
