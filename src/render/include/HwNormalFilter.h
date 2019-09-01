/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_HWNORMALFILTER_H
#define HWVC_ANDROID_HWNORMALFILTER_H

#include "HwAbsFilter.h"

class HwNormalFilter : public HwAbsFilter {
public:
    HwNormalFilter();

    virtual ~HwNormalFilter();

    virtual bool init() override;

    virtual void draw(HwAbsTexture *src, HwAbsTexture *dest) override;

};


#endif //HWVC_ANDROID_HWNORMALFILTER_H
