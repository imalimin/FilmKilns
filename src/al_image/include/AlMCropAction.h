/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALMCROPACTION_H
#define HWVC_ANDROID_ALMCROPACTION_H

#include "AlAbsMAction.h"
#include "AlRectF.h"
#include "AlRational.h"
#include "AlLayerMeasurer.h"

al_class_ex(AlMCropAction, AlAbsMAction) {
public:
    AlMCropAction();

    AlMCropAction(const AlMCropAction &o);

    virtual ~AlMCropAction();

    virtual HwResult measure(AlImgLayerDescription &layer,
                             AlImageLayerDrawModel *description) override;

    void setRect(float left, float top, float right, float bottom);

    virtual HwResult fromElement(AlElement *element) override;

    virtual HwResult toElement(AlElement **element) override;

private:
    bool invalidate = false;
    /// 裁剪框区域，相对于画布坐标系
    AlRectF rectF;
    AlVec2 scale;
    AlRational rotation;
    AlVec2 position;

    AlSize cropSize;
    AlQuad quad;
    AlLayerMeasurer aMeasurer = AlLayerMeasurer::centerInside();
};


#endif //HWVC_ANDROID_ALMCROPACTION_H
