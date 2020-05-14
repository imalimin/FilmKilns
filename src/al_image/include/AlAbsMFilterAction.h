/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALABSMFILTERACTION_H
#define HWVC_ANDROID_ALABSMFILTERACTION_H

#include "AlAbsMAction.h"

al_class AlAbsMFilterAction al_extend AlAbsMAction {
public:
    AlAbsMFilterAction(std::string type);

    ~AlAbsMFilterAction();

    virtual HwResult draw(HwAbsTexture *src, HwAbsTexture *dest) = 0;

private:
    HwResult measure(AlImgLayerDescription &layer, AlImageLayerDrawModel *description) override;

};


#endif //HWVC_ANDROID_ALABSMFILTERACTION_H
