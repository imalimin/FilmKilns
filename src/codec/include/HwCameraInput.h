/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_HWCAMERAINPUT_H
#define HWVC_ANDROID_HWCAMERAINPUT_H

#include "Unit.h"
#include "AlEgl.h"
#include "HwTexture.h"
#include "HwAbsFBObject.h"
#include "HwProgram.h"

class HwCameraInput : public Unit {
public:
    HwCameraInput(string alias);

    virtual ~HwCameraInput();

private:
    bool onCreate(AlMessage *msg) override;

    bool onDestroy(AlMessage *msg) override;

    bool eventInvalidate(AlMessage *msg);

    void _onUpdateSize(AlMessage *msg);

    void _onRun(AlMessage *msg);

private:
    void draw(int w, int h);

    void notify(int64_t tsInNs, int w, int h);

    void updateMatrix(int32_t w, int32_t h, AlMatrix *matrix);

private:
    AlSize cameraSize;
    AlEgl *egl = nullptr;
    AlSPointer<HwTexture> srcTex;
    HwTexture *destTex = nullptr;
    HwAbsFBObject *fbo = nullptr;
    HwProgram *program = nullptr;
};


#endif //HWVC_ANDROID_HWCAMERAINPUT_H
