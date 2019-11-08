/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_HWABSGRAPHICBUFFER_H
#define HWVC_ANDROID_HWABSGRAPHICBUFFER_H

#include "Object.h"

class HwAbsGraphicBuffer : public Object {
public:
    HwAbsGraphicBuffer(int w, int h);

    virtual ~HwAbsGraphicBuffer();

    virtual bool bind() = 0;

    virtual bool read(uint8_t *pixels) = 0;

protected:
    int width = 0;
    int height = 0;
};


#endif //HWVC_ANDROID_HWABSGRAPHICBUFFER_H
