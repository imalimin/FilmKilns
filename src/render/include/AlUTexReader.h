/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALUTEXREADER_H
#define HWVC_ANDROID_ALUTEXREADER_H

#include "Unit.h"
#include "HwAbsFilter.h"

al_def_unit(AlUTexReader, Unit) {
public:
    AlUTexReader(const string &alias);

    virtual ~AlUTexReader();

    bool onCreate(AlMessage *msg) override;

    bool onDestroy(AlMessage *msg) override;

private:
    bool _onScreenDraw(AlMessage *msg);

    /// 读取图层pixels
    /// FORMAT:
    /// +--------------------------------------------------+
    /// | msg::obj     | msg::arg1 | msg::arg2 | msg::desc |
    /// +--------------------------------------------------+
    /// | none         | pixels    | none      | out path  |
    /// +--------------------------------------------------+
    /// \param msg
    /// \return
    bool _onReqPixels(AlMessage *msg);

private:
    HwAbsTexture *srcTex = nullptr;
    HwAbsFilter *yuvFilter = nullptr;
    HwAbsTexture *yuvTex = nullptr;
    HwAbsFBObject *fbo = nullptr;
    AlBuffer *pixels = nullptr;
};


#endif //HWVC_ANDROID_ALUTEXREADER_H
