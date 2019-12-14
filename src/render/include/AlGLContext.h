/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALGLCONTEXT_H
#define HWVC_ANDROID_ALGLCONTEXT_H

#include "AlContext.h"
#include "TextureAllocator.h"

al_class_ex(AlGLContext, AlContext) {
public:
    AlGLContext();

    ~AlGLContext();

    TextureAllocator *getTextureAllocator();

private:
    TextureAllocator *texAllocator = nullptr;
};


#endif //HWVC_ANDROID_ALGLCONTEXT_H
