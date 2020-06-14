/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALMEDIACLIP_H
#define HWVC_ANDROID_ALMEDIACLIP_H

#include "Object.h"

AL_CLASS AlMediaClip AL_EXTEND Object {
public:
    AlMediaClip();

    AlMediaClip(const AlMediaClip &o);

    virtual ~AlMediaClip();

private:
    int32_t _id = INT32_MIN;
};


#endif //HWVC_ANDROID_ALMEDIACLIP_H
