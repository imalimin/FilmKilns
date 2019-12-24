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
#include "AlTexAllocator.h"
#include "AlBitmapFactory.h"

class Image : public Unit {
public:
    Image(string alias);

    virtual ~Image();

    bool onDestroy(AlMessage *msg) override;

    bool onCreate(AlMessage *msg) override;

    bool eventShow(AlMessage *msg);

    bool eventInvalidate(AlMessage *msg);

private:
    AlTexAllocator *texAllocator = nullptr;
    AlBitmap *hwBitmap = nullptr;
    HwAbsTexture *tex = nullptr;

    void show(string path);

    bool decode(string path);
};


#endif //HARDWAREVIDEOCODEC_IMAGE_H
