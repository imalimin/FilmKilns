/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_HWABSFBOBJECT_H
#define HWVC_ANDROID_HWABSFBOBJECT_H

#include "Object.h"
#include "Size.h"
#include "AlBuffer.h"

class HwAbsTexture;

class HwAbsFBObject : public Object {
public:
    HwAbsFBObject();

    virtual ~HwAbsFBObject();

    void bindTex(HwAbsTexture *tex);

    void unbindTex();

    void bind();

    void unbind();

    virtual bool read(uint8_t *pixels);

    virtual bool read(AlBuffer *pixels);

private:
    uint32_t fbo = 0;
    HwAbsTexture *tex = nullptr;
};


#endif //HWVC_ANDROID_HWABSFBOBJECT_H
