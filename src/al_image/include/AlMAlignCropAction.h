/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALMALIGNCROPACTION_H
#define HWVC_ANDROID_ALMALIGNCROPACTION_H

#include "AlAbsMAction.h"

al_class AlMAlignCropAction al_extend AlAbsMAction {
public:
    AlMAlignCropAction();

    AlMAlignCropAction(const AlMAlignCropAction &o);

    virtual ~AlMAlignCropAction();

    virtual HwResult measure(AlImgLayerDescription &layer,
                             AlImageLayerDrawModel *description) override;

    virtual HwResult fromElement(AlElement *element) override;

    virtual HwResult toElement(AlElement **element) override;

    void setRotation(AlRational &r);

    AlRational getRotation();

private:
    AlRational rotation;
};


#endif //HWVC_ANDROID_ALMALIGNCROPACTION_H
