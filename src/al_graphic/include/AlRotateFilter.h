/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALROTATEFILTER_H
#define HWVC_ANDROID_ALROTATEFILTER_H

#include "HwNormalFilter.h"
#include "AlRational.h"

///旋转纹理，只支持90的整数倍
al_class AlRotateFilter al_extend HwNormalFilter {
public:
    AlRotateFilter();

    virtual ~AlRotateFilter();

    virtual void drawFirst(AlAbsGLProgram *program, HwAbsTexture *src, HwAbsTexture *dest) override;

    void setRotation(AlRational &r);

private:
    AlRational rotation = AlRational();
};


#endif //HWVC_ANDROID_ALROTATEFILTER_H
