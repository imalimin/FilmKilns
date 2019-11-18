/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */
#ifndef HARDWAREVIDEOCODEC_IMAGE_H
#define HARDWAREVIDEOCODEC_IMAGE_H

#include "Unit.h"
#include "BaseDrawer.h"
#include "TextureAllocator.h"
#include "HwBitmapFactory.h"

class Image : public Unit {
public:
    Image(string alias);

    virtual ~Image();

    bool eventRelease(Message *msg) override;

    bool eventPrepare(Message *msg);

    bool eventShow(Message *msg);

    bool eventInvalidate(Message *msg);

private:
    TextureAllocator *texAllocator = nullptr;
    HwBitmap *hwBitmap = nullptr;
    HwAbsTexture *tex = nullptr;

    void show(string path);

    bool decode(string path);
};


#endif //HARDWAREVIDEOCODEC_IMAGE_H
