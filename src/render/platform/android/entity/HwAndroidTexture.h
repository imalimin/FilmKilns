/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_HWANDROIDTEXTURE_H
#define HWVC_ANDROID_HWANDROIDTEXTURE_H

#include "../../../include/HwAbsTexture.h"
#include "../../../include/HwAbsTexture.h"

class HwAndroidTexture : public HwAbsTexture {
public:
    HwAndroidTexture(uint32_t target);

    virtual ~HwAndroidTexture();

    virtual void bind() override;

    virtual void unbind() override;

    virtual void update(HwBuffer *buf, int32_t w, int32_t h) override;

public:
    virtual bool read(uint8_t *pixels) override;

private:
    bool support();

private:
    int sdk = 0;
    Object *graphicBuffer = nullptr;
    HwAbsTexture *texCompat = nullptr;
};


#endif //HWVC_ANDROID_HWANDROIDTEXTURE_H
