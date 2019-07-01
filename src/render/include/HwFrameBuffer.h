/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_HWFRAMEBUFFER_H
#define HWVC_ANDROID_HWFRAMEBUFFER_H

#include "HwAbsFrameBuffer.h"

class HwFrameBuffer : public HwAbsFrameBuffer {
public:
    HwFrameBuffer(int w, int h);

    virtual ~HwFrameBuffer();

protected:
    virtual void createTexture() override;
};


#endif //HWVC_ANDROID_HWFRAMEBUFFER_H
