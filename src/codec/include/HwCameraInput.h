/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_HWCAMERAINPUT_H
#define HWVC_ANDROID_HWCAMERAINPUT_H

#include "Unit.h"
#include "Egl.h"
#include "HwTexture.h"
#include "HwAbsFBObject.h"
#include "HwProgram.h"

class HwCameraInput : public Unit {
public:
    HwCameraInput(string alias);

    virtual ~HwCameraInput();

    bool eventPrepare(Message *msg);

    bool eventRelease(Message *msg) override;

    bool eventInvalidate(Message *msg);

    uint32_t getTex();

    void mackCurrent();

    void calculateBestLocation(int32_t vw, int32_t vh, int32_t w, int32_t h);

private:
    void draw(int w, int h);

    void notify(int64_t tsInNs, int w, int h);

private:
    Egl *egl = nullptr;
    HwTexture *srcTex = nullptr;
    HwTexture *destTex = nullptr;
    HwAbsFBObject *fbo = nullptr;
    HwProgram *program = nullptr;
};


#endif //HWVC_ANDROID_HWCAMERAINPUT_H
