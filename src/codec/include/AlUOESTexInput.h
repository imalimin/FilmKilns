/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALUOESTEXINPUT_H
#define HWVC_ANDROID_ALUOESTEXINPUT_H

#include "Unit.h"
#include "AlEgl.h"
#include "HwTexture.h"
#include "HwAbsFBObject.h"
#include "HwProgram.h"
#include "AlRational.h"

al_def_unit(AlUOESTexInput, Unit) {
public:
    AlUOESTexInput(string alias);

    virtual ~AlUOESTexInput();

private:
    bool onCreate(AlMessage *msg) override;

    bool onDestroy(AlMessage *msg) override;

    bool _onInvalidate(AlMessage *msg);

    bool _onUpdateSize(AlMessage *msg);

    bool _onRun(AlMessage *msg);

    bool _onLayerNotify(AlMessage *msg);

    bool _onOutputSize(AlMessage *msg);

    bool _onScale(AlMessage *msg);

private:
    void draw();

    void notify(int64_t tsInNs);

    void updateMatrix(int32_t w, int32_t h, AlMatrix *matrix);

    void _createLayer();

private:
    AlSize cameraSize;
    AlSize outSize;
    AlRational scale;
    AlSPointer<HwAbsTexture> srcTex;
    HwAbsFBObject *fbo = nullptr;
    HwProgram *program = nullptr;
    HwAbsTexture *mLayerTex = nullptr;
    int32_t mLayerId = -1;

};


#endif //HWVC_ANDROID_ALUOESTEXINPUT_H
