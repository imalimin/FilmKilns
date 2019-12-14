/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlGLContext.h"

AlGLContext::AlGLContext() {
    texAllocator = new TextureAllocator();
}

AlGLContext::~AlGLContext() {
    delete texAllocator;
    texAllocator = nullptr;
}

TextureAllocator *AlGLContext::getTextureAllocator() {
    return texAllocator;
}
