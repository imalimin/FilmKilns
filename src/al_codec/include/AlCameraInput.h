/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALCAMERAINPUT_H
#define HWVC_ANDROID_ALCAMERAINPUT_H

#include "Unit.h"
#include "AlEgl.h"
#include "HwTexture.h"
#include "HwAbsFBObject.h"
#include "HwProgram.h"
#include "AlFPSMeasurer.h"

AL_CLASS AlCameraInput AL_EXTEND Unit {
public:
    AlCameraInput(string alias);

    virtual ~AlCameraInput();

private:
    bool onCreate(AlMessage *msg) override;

    bool onDestroy(AlMessage *msg) override;

    bool _onInvalidate(AlMessage *msg);

    bool _onUpdateSize(AlMessage *msg);

    bool _onRun(AlMessage *msg);

    bool _onLayerNotify(AlMessage *msg);

    bool _onOutputSize(AlMessage *msg);

private:
    void draw();

    void notify(int64_t tsInNs);

    void updateMatrix(int32_t w, int32_t h, AlMatrix *matrix);

private:
    AlSize cameraSize;
    AlSize outSize;
    HwAbsTexture *srcTex;
    HwAbsFBObject *fbo = nullptr;
    HwProgram *program = nullptr;
    HwAbsTexture *mLayerTex = nullptr;
    int32_t mLayerId = -1;
    AlFPSMeasurer fps;
};


#endif //HWVC_ANDROID_ALCAMERAINPUT_H
