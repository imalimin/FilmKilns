/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALIMAGE_H
#define HWVC_ANDROID_ALIMAGE_H

#include "Unit.h"
#include "TextureAllocator.h"
#include "HwAbsTexture.h"

class AlImage : public Unit {
public:
    AlImage(string alias);

    virtual ~AlImage();

    bool eventRelease(Message *msg) override;

    bool onPrepare(Message *msg);

    bool onNewLayer(Message *msg);

private:
    TextureAllocator *texAllocator = nullptr;
    HwAbsTexture *tex = nullptr;
};


#endif //HWVC_ANDROID_ALIMAGE_H
