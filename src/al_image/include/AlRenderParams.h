/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALRENDERPARAMS_H
#define HWVC_ANDROID_ALRENDERPARAMS_H

#include "AlBinaryParams.h"

/// flags:
/// +------------------------------------------------------------+
/// | index     | value 0                 | value 1              |
/// +------------------------------------------------------------+
/// | 0         | Draw to screen          | Don`t draw to screen |
/// +------------------------------------------------------------+
/// | 1         | default canvas bg       | transparent canvas   |
/// +------------------------------------------------------------+
/// | 2         | none                    | save as jpg/png/webp |
/// +------------------------------------------------------------+
al_class_ex(AlRenderParams, AlBinaryParams) {
public:
    AlRenderParams(int32_t flags = 0);

    AlRenderParams(const AlRenderParams &o);

    virtual  ~AlRenderParams();

    void setRenderScreen(bool value);

    bool isRenderScreen();

    void setTransparent(bool value);

    bool isTransparent();

    void setReqSave(bool value);

    bool isReqSave();

    void setReqClear(bool value);

    bool isReqClear();
};


#endif //HWVC_ANDROID_ALRENDERPARAMS_H
